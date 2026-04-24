#include <cube/data/World.hpp>

#include <mutex>

#include "cube/utils/Logger.hpp"

namespace cube {

    World::World(const int seed) : m_generator(seed) {}

    Chunk* World::getChunk(const glm::ivec3& chunkPos) {
        std::shared_lock lock(m_mapMutex);
        const auto it = m_chunks.find(chunkPos);
        if (it != m_chunks.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    std::vector<glm::ivec3> World::loadArea(const glm::ivec3& centerChunk, const int radius) {
        std::vector<glm::ivec3> new_chunks;

        std::unique_lock lock(m_mapMutex);

        for (int x = -radius; x <= radius; ++x) {
            for (int z = -radius; z <= radius; ++z) {
                for (int y = 0; y < 16; ++y) {
                    glm::ivec3 targetPos = {centerChunk.x + x, y, centerChunk.z + z};

                    if (m_chunks.find(targetPos) == m_chunks.end()) {
                        m_chunks[targetPos] = std::make_unique<Chunk>(m_generator.generate(targetPos));
                        new_chunks.push_back(targetPos);
                    }
                }
            }
        }
        return new_chunks;
    }

    void World::unloadFarChunks(const glm::ivec3 &centerChunk, const int radiusSq, const std::function<void(const glm::ivec3 &)>& onUnload) {
        int removedCount = 0;
        std::unique_lock lock(m_mapMutex);

        for (auto it = m_chunks.begin(); it != m_chunks.end(); ) {
            if (dist(it->first, centerChunk) > radiusSq) {
                if (onUnload) onUnload(it->first);
                it = m_chunks.erase(it);
                removedCount++;
            } else {
                ++it;
            }
        }

        if (removedCount > 0) {
            Log::info("Removed {} chunks", removedCount);
        }
    }

    Block World::getBlock(const glm::ivec3& worldPos) {
        if (worldPos.y < 0 || worldPos.y >= 256) return Air;

        const glm::ivec3 cPos = {worldPos.x >> 4, worldPos.y >> 4, worldPos.z >> 4};
        std::shared_lock lock(m_mapMutex);

        if (const auto it = m_chunks.find(cPos); it != m_chunks.end() && it->second) {
            return it->second->view()[worldPos.x & 15, worldPos.y & 15, worldPos.z & 15];
        }
        return Air;
    }

    ChunkNeighbors World::getNeighbors(const glm::ivec3 &pos) {
        ChunkNeighbors neighbors{};
        std::shared_lock lock(m_mapMutex);

        if (const auto it = m_chunks.find(pos); it != m_chunks.end()) neighbors.center = it->second.get();
        if (const auto i = m_chunks.find(pos + glm::ivec3(1,0,0)); i != m_chunks.end()) neighbors.px = i->second.get();
        if (const auto i = m_chunks.find(pos + glm::ivec3(0,1,0)); i != m_chunks.end()) neighbors.py = i->second.get();
        if (const auto i = m_chunks.find(pos + glm::ivec3(0,0,1)); i != m_chunks.end()) neighbors.pz = i->second.get();
        if (const auto i = m_chunks.find(pos - glm::ivec3(1,0,0)); i != m_chunks.end()) neighbors.nx = i->second.get();
        if (const auto i = m_chunks.find(pos - glm::ivec3(0,1,0)); i != m_chunks.end()) neighbors.ny = i->second.get();
        if (const auto i = m_chunks.find(pos - glm::ivec3(0,0,1)); i != m_chunks.end()) neighbors.nz = i->second.get();

        return neighbors;
    }

    void World::ensureChunkExists(const glm::ivec3& pos) {
        {
            std::shared_lock lock(m_mapMutex);
            if (m_chunks.contains(pos)) return;
        }

        std::unique_lock lock(m_mapMutex);
        if (!m_chunks.contains(pos)) {
            //Log::info("Chunk [{},{},{}] is generating", pos.x, pos.y, pos.z);
            m_chunks[pos] = std::make_unique<Chunk>(m_generator.generate(pos));
        }
    }

}
