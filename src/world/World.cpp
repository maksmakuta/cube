#include "cube/world/World.hpp"

#include <algorithm>
#include <ranges>

#include "cube/core/Constants.hpp"
#include "cube/generators/GeneratorFlat.hpp"
#include "cube/utils/Utils.hpp"

namespace cube {
    World::World(const int seed) : m_generator(std::make_unique<GeneratorFlat>()) {
        setSeed(seed);
    }

    World::~World() = default;

    void World::onTick(ThreadPool &pool, const Player &player) {
        const auto center = toChunk(player.getPosition());
        std::unordered_set<glm::ivec2> needed;

        for (int dz = -RENDER_DIST; dz <= RENDER_DIST; ++dz) {
            for (int dx = -RENDER_DIST; dx <= RENDER_DIST; ++dx) {
                const auto cp = center + glm::ivec2(dx, dz);
                needed.insert(cp);

                if (!m_chunks.contains(cp)) {
                    pool.submit([this, cp]() {
                        const auto new_chunk = m_generator->getChunk(cp);
                        std::lock_guard lock(m_qmutex);
                        m_results.emplace(cp, new_chunk);
                    });
                }
            }
        } {
            std::lock_guard inner_lock(m_qmutex);
            while (!m_results.empty()) {
                if (auto [pos, data] = m_results.front(); !m_chunks.contains(pos)) {
                    m_chunks.insert_or_assign(pos, data);
                }
                m_results.pop();
            }
        }

        for (auto it = m_chunks.begin(); it != m_chunks.end();) {
            if (!needed.contains(it->first)) {
                it = m_chunks.erase(it);
            } else {
                ++it;
            }
        }
    }

    void World::onUpdate(const Player &player) {
        center = player.getPosition();
    }

    std::vector<glm::ivec2> World::getChunks() const {
        const auto l = std::views::keys(m_chunks);
        auto vec = std::vector<glm::ivec2>{l.begin(), l.end()};
        std::ranges::sort(vec, [](const glm::ivec2 &a, const glm::ivec2 &b) {
            return a.x > b.x && a.y > b.y;
        });

        return vec;
    }

    ChunkPtr World::getChunk(const glm::ivec2 &p) const {
        if (m_chunks.contains(p)) {
            //lock here
            return m_chunks.at(p);
        }
        return nullptr;
    }

    int World::getSeed() const {
        return m_generator->getSeed();
    }

    void World::setSeed(const int s) {
        m_generator->setSeed(s);
    }

    void World::setGenerator(std::unique_ptr<IGenerator> new_generator) {
        const auto seed = m_generator->getSeed();
        m_generator = std::move(new_generator);
        m_generator->setSeed(seed);
    }
}
