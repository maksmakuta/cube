#include "cube/world/World.hpp"

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
        //m_visible.clear();

        for (int dz = -RENDER_DIST; dz <= RENDER_DIST; ++dz) {
            for (int dx = -RENDER_DIST; dx <= RENDER_DIST; ++dx) {
                const auto cp = center + glm::ivec2(dx, dz);
                needed.insert(cp);
                //
                // const auto cc = glm::vec3(glm::ivec3{dx, 0, dz} * CHUNK_ORIGIN + CHUNK_CENTER);
                // auto toChunk = glm::normalize(glm::vec2(cc.x - player.getPosition().x, cc.z - player.getPosition().z));
                // auto forward = glm::vec2(cos(glm::radians(player.getRotation().x)), sin(glm::radians(player.getRotation().x)));
                // const float dot = glm::dot(toChunk, forward); // 1 = perfect forward
                //
                // const float maxDot = cosf(glm::radians(45.f) * 0.5f);
                // if (dot >= maxDot) {
                //     m_visible.insert(cp);
                // }

                if (!m_chunks.contains(cp)) {
                    //lock here and put into pool
                    auto chunk = m_generator->getChunk(cp);
                    m_chunks[cp] = std::move(chunk);

                    pool.submit([=]() {
                        const auto new_chunk = m_generator->getChunk(cp);
                        std::lock_guard lock(m_qmutex);
                        m_results.emplace(cp,new_chunk);
                    });
                }
            }
        }

        {
            std::lock_guard inner_lock(m_qmutex);
            while(!m_results.empty()) {
                auto [pos, data] = m_results.front();
                if (!m_chunks.contains(pos)) {
                    m_chunks.insert_or_assign(pos,data);
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

    std::unordered_set<glm::ivec2> World::getVisibleChunks() const {
        return m_visible;
    }

    std::unordered_set<glm::ivec2> World::getChunks() const {
        const auto l = std::views::keys(m_chunks);
        return {l.begin(), l.end()};
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
}
