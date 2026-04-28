#include "cube/data/World.hpp"

namespace cube {

    void World::insertChunk(const glm::ivec3& pos, std::unique_ptr<Chunk> chunk) {
        std::lock_guard lock(m_uploadMutex);
        m_pendingChunks.push_back({pos, std::move(chunk)});
    }

    std::vector<glm::ivec3> World::processPendingChunks() {
        std::vector<PendingChunk> localChunks;
        {
            std::lock_guard lock(m_uploadMutex);
            localChunks.swap(m_pendingChunks);
        }

        std::vector<glm::ivec3> newlyAdded;
        newlyAdded.reserve(localChunks.size());

        {
            std::unique_lock writeLock(m_worldMutex);
            for (auto&[pos, chunk] : localChunks) {
                m_chunks[pos] = std::shared_ptr<Chunk>(chunk.release());
                newlyAdded.push_back(pos);
            }
        }
        return newlyAdded;
    }

    std::shared_ptr<const Chunk> World::getChunkReadonly(const glm::ivec3& pos) const {
        std::shared_lock readLock(m_worldMutex);
        if (const auto it = m_chunks.find(pos); it != m_chunks.end()) {
            return it->second;
        }
        return nullptr;
    }

}