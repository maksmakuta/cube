#include "cube/data/World.hpp"

namespace cube {

    World::World() = default;

    Status World::getStatus(const glm::ivec3& pos) {
        if (!m_statuses.contains(pos)) {
            m_statuses[pos] = Status::Requested;
        }
        return m_statuses[pos];
    }

    void World::setStatus(const glm::ivec3& pos, const Status status) {
        m_statuses[pos] = status;
    }

    std::shared_ptr<Chunk> World::getChunk(const glm::ivec3& pos) {
        return m_chunks.contains(pos) ? m_chunks[pos] : nullptr;
    }

    void World::setChunk(const glm::ivec3& pos, std::shared_ptr<Chunk> chunk) {
        m_chunks[pos] = std::move(chunk);
    }

}