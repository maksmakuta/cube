#include <cube/data/World.hpp>

#include <mutex>

namespace cube {

    World::World(const int seed) : m_generator(seed) {}

    Chunk* World::getChunk(const glm::ivec3& chunkPos) {
        std::shared_lock lock(m_mapMutex);
        const auto it = m_chunks.find(chunkPos);
        if (it != m_chunks.end()) {
            return &it->second;
        }
        return nullptr;
    }

    void World::loadArea(const glm::ivec3& centerChunk, int radius) {
        for (int x = -radius; x <= radius; ++x) {
            for (int z = -radius; z <= radius; ++z) {
                for (int y = 0; y < 16; ++y) {
                    glm::ivec3 targetPos = {centerChunk.x + x, y, centerChunk.z + z};
                    ensureChunkExists(targetPos);
                }
            }
        }
    }

    Block World::getBlock(const glm::ivec3& worldPos) {
        const glm::ivec3 cPos = {worldPos.x >> 4, worldPos.y >> 4, worldPos.z >> 4};

        if (cPos.y < 0 || cPos.y >= 16) return Air;

        std::shared_lock lock(m_mapMutex);
        if (const auto it = m_chunks.find(cPos); it != m_chunks.end()) {
            return it->second.view()[worldPos.x & 15, worldPos.y & 15, worldPos.z & 15];
        }
        return Air;
    }

    void World::ensureChunkExists(const glm::ivec3& pos) {
        {
            std::shared_lock lock(m_mapMutex);
            if (m_chunks.contains(pos)) return;
        }

        std::unique_lock lock(m_mapMutex);
        if (!m_chunks.contains(pos)) {
            m_chunks.emplace(pos, m_generator.generate(pos));
        }
    }

}