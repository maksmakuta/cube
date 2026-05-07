#include "cube/data/World.hpp"

namespace cube {

    World::World() = default;

    bool World::contains(const glm::ivec3& pos) const {
        return m_chunks.contains(pos);
    }

    ChunkStatus World::getStatus(const glm::ivec3& pos) const {
        if (m_statuses.contains(pos)) {
            return m_statuses.at(pos);
        }
        return ChunkStatus::Unknown;
    }

    ChunkPtr World::getChunk(const glm::ivec3& pos) const {
        if (m_chunks.contains(pos)) {
            return m_chunks.at(pos);
        }
        return nullptr;
    }

    void World::setStatus(const glm::ivec3& pos, const ChunkStatus status) {
        m_statuses[pos] = status;
    }

    void World::setChunk(const glm::ivec3& pos, ChunkPtr chunk) {
        m_chunks[pos] = std::move(chunk);
    }

}