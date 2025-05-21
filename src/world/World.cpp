#include "cube/world/World.hpp"

#include <iostream>
#include <random>
#include <glm/gtx/string_cast.hpp>

#include "cube/generators/GeneratorFlat.hpp"
#include "cube/generators/GeneratorTerrain.hpp"
#include "cube/utils/Utils.hpp"

namespace cube {

    World::World(const int seed) : m_generator(std::make_shared<GeneratorTerrain>()),
        m_seed(seed == 0 ? static_cast<int>(std::random_device{}()) : seed) {}

    World::~World() = default;

    void World::setGenerator(const std::shared_ptr<IGenerator>& g) {
        m_generator = g;
        m_generator->setSeed(m_seed);
    }

    std::shared_ptr<IGenerator> World::getGenerator() const {
        return m_generator;
    }

    void World::onTick(ThreadPool& pool,const glm::vec3& player_pos) {
        const auto center = toChunk(player_pos);
        std::unordered_set<glm::ivec2> activeChunks;

        for (int dx = -RENDER_DIST; dx < RENDER_DIST; ++dx) {
            for (int dz = -RENDER_DIST; dz < RENDER_DIST; ++dz) {
                glm::ivec2 pos = center + glm::ivec2(dx, dz);
                activeChunks.insert(pos);

                {
                    std::lock_guard lock(m_mutex);
                    if (m_chunks.contains(pos))
                        continue;
                }

                pool.submit([this, pos] {
                    const ChunkPtr chunk = getGenerator()->generateAt(pos);
                    if (chunk) {
                        std::lock_guard lock(m_mutex);
                        m_chunks.insert({pos, chunk});
                    }
                });
            }
        }

        {
            std::lock_guard lock(m_mutex);
            for (auto it = m_chunks.begin(); it != m_chunks.end(); ) {
                if (!activeChunks.contains(it->first)) {
                    it = m_chunks.erase(it);
                } else {
                    ++it;
                }
            }
        }

        std::cout << "Size : " << m_chunks.size() << std::endl;

    }

    void World::forChunk(const std::function<void(const ChunkPtr&, const glm::ivec2&)> &fn) {
        std::lock_guard lock(m_mutex);
        for (auto & [pos, chunk]: m_chunks) {
            fn(chunk,pos);
        }
    }

    int World::getSeed() const {
        return m_seed;
    }

    void World::setSeed(const int seed){
        m_seed = seed;
        if(m_generator) {
            m_generator->setSeed(seed);
        }
    }

    ChunkPtr World::at(const glm::ivec2& v) {
        std::shared_lock lock(m_mutex);
        const auto it = m_chunks.find(v);
        return it != m_chunks.end() ? it->second : nullptr;
    }
}
