#include "cube/data/World.hpp"

#include <algorithm>
#include <mutex>

namespace cube {

    World::World() = default;

    Status World::getStatus(const glm::ivec3& pos) {
        std::unique_lock lock(m_worldMutex);
        if (!m_statuses.contains(pos)) {
            m_statuses[pos] = Status::Requested;
        }
        return m_statuses[pos];
    }

    void World::setStatus(const glm::ivec3& pos, const Status status) {
        std::unique_lock lock(m_worldMutex);
        m_statuses[pos] = status;
    }

    std::shared_ptr<Chunk> World::getChunk(const glm::ivec3& pos) const {
        std::shared_lock lock(m_worldMutex);
        return m_chunks.contains(pos) ? m_chunks.at(pos) : nullptr;
    }

    void World::setChunk(const glm::ivec3& pos, std::shared_ptr<Chunk> chunk) {
        std::unique_lock lock(m_worldMutex);
        m_chunks[pos] = std::move(chunk);
    }

    void World::unloadChunks(const glm::ivec3 &pos, const int render_distance) {
        std::unique_lock lock(m_worldMutex);

        auto chunkIt = m_chunks.begin();
        while (chunkIt != m_chunks.end()) {
            const auto distance = static_cast<int>(glm::floor(glm::length(glm::vec3(chunkIt->first - pos))));
            if (distance > render_distance) {
                chunkIt = m_chunks.erase(chunkIt);
            } else {
                ++chunkIt;
            }
        }

        auto statusIt = m_statuses.begin();
        while (statusIt != m_statuses.end()) {
            const auto distance = static_cast<int>(glm::floor(glm::length(glm::vec3(statusIt->first - pos))));
            if (distance > render_distance) {
                statusIt = m_statuses.erase(statusIt);
            } else {
                ++statusIt;
            }
        }
    }

    bool World::readyToMesh(const glm::ivec3& pos) const {
        std::shared_lock lock(m_worldMutex);
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                for (int dz = -1; dz <= 1; dz++) {
                    if (!m_chunks.contains(pos + glm::ivec3(dx, dy, dz))) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

}
