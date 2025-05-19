#include "cube/world/World.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>

#include "cube/generators/GeneratorFlat.hpp"
#include "cube/generators/GeneratorTerrain.hpp"
#include "cube/utils/Utils.hpp"

namespace cube {

    World::World(const int seed) : m_generator(std::make_shared<GeneratorTerrain>()), m_seed(seed) {}

    World::~World() = default;

    void World::setGenerator(const std::shared_ptr<IGenerator>& g) {
        m_generator = g;
        m_generator->setSeed(m_seed);
    }

    std::shared_ptr<IGenerator> World::getGenerator() const {
        return m_generator;
    }

    void World::insert(const Chunk& c) {
        m_chunks.insert_or_assign(c.getOffset(),c);
    }

    Chunk World::at(const glm::ivec2 &pos) const {
        if (const auto it = m_chunks.find(pos); it != m_chunks.end()) return it->second;
        throw std::out_of_range("Chunk not found");
    }

    void World::remove(const glm::ivec2& pos) {
        m_chunks.erase(pos);
    }

    void World::onTick(ThreadPool& pool,const glm::vec3& pos) {
        const auto center = toChunk(pos);
        std::unordered_map<glm::ivec2, Chunk> newChunks;

        for (int dx = -RENDER_DIST; dx <= RENDER_DIST; ++dx) {
            for (int dz = -RENDER_DIST; dz <= RENDER_DIST; ++dz) {
                glm::ivec2 coord = center + glm::ivec2(dx, dz);
                if (const auto it = m_chunks.find(coord); it != m_chunks.end()) {
                    newChunks.emplace(coord, std::move(it->second));
                } else {
                     pool.submit([this, coord] {
                        auto chunk = m_generator->generateAt(coord);
                         std::lock_guard lock(m_chunks_mutex);
                        m_new_chunks.emplace(std::move(chunk));
                    });
                }
            }
        }

        {
            std::lock_guard lock(m_chunks_mutex);
            while (!m_new_chunks.empty()){
                const auto item = m_new_chunks.front();
                newChunks[item.getOffset()] = item;
                m_new_chunks.pop();
            }
        }

        m_chunks = std::move(newChunks);
    }

    void World::forChunk(const std::function<void(Chunk &)> &fn) {
        for (auto &item: m_chunks | std::views::values) {
            fn(item);
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

}
