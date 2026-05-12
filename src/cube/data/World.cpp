#include "cube/data/World.hpp"

#include <mutex>

namespace cube {

    World::World() = default;

    bool World::contains(const glm::ivec3& pos) const {
        std::shared_lock lock(m_mutex);
        return m_chunks.contains(pos);
    }

    ChunkStatus World::getStatus(const glm::ivec3& pos) const {
        std::shared_lock lock(m_mutex);
        if (m_statuses.contains(pos)) {
            return m_statuses.at(pos);
        }
        return ChunkStatus::Unknown;
    }

    ChunkPtr World::getChunk(const glm::ivec3& pos) const {
        std::shared_lock lock(m_mutex);
        if (m_chunks.contains(pos)) {
            return m_chunks.at(pos);
        }
        return nullptr;
    }

    void World::setStatus(const glm::ivec3& pos, const ChunkStatus status) {
        std::unique_lock lock(m_mutex);
        m_statuses[pos] = status;
    }

    void World::setChunk(const glm::ivec3& pos, ChunkPtr chunk) {
        std::unique_lock lock(m_mutex);
        m_chunks[pos] = std::move(chunk);
    }

    int World::clearChunks(const glm::ivec3& pos, const int dist) {
        std::unique_lock lock(m_mutex);
        const auto initialCount = m_chunks.size();

        auto isOutOfRange = [&](const auto& item) {
            const glm::ivec3& chunkPos = item.first;
            return std::abs(chunkPos.x - pos.x) > dist ||
                   std::abs(chunkPos.y - pos.y) > dist ||
                   std::abs(chunkPos.z - pos.z) > dist;
        };

        std::erase_if(m_chunks, isOutOfRange);
        std::erase_if(m_statuses, isOutOfRange);

        return static_cast<int>(initialCount - m_chunks.size());
    }

}
