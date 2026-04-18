#include <algorithm>
#include <cube/data/World.hpp>

namespace cube {

    World::World(const int seed) : m_seed(seed) {
        // In a future step, initialize your noise generator here
    }

    Chunk* World::getChunk(const glm::ivec3& chunkPos) {
        auto it = m_chunks.find(chunkPos);
        if (it != m_chunks.end()) {
            return &it->second;
        }
        return nullptr;
    }

    bool World::putChunk(const glm::ivec3& chunkPos, Chunk&& chunk) {
        // move the chunk into the map
        auto [it, inserted] = m_chunks.insert_or_assign(chunkPos, std::move(chunk));
        return inserted;
    }

    void World::unloadDistantChunks(const glm::ivec3& playerChunkPos, int viewDistance) {
        // C++20/23 highly efficient way to remove elements from a map while iterating
        std::erase_if(m_chunks, [&](const auto& item) {
            const auto& [pos, chunk] = item;
            // Check Manhattan distance or Euclidean distance
            float dist = glm::distance(glm::vec3(pos), glm::vec3(playerChunkPos));
            return dist > static_cast<float>(viewDistance);
        });
    }

    void World::generateChunkData(const glm::ivec3& chunkPos, Chunk& chunk) {
        auto view = chunk.getMutableView();

        for (int x = 0; x < CHUNK_SIZE; ++x) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                // Global world coordinates for noise/math
                int worldX = chunkPos.x * CHUNK_SIZE + x;
                int worldZ = chunkPos.z * CHUNK_SIZE + z;

                // Simple "Sine Wave" terrain for testing
                // We scale coordinates down to make the hills "stretch"
                float heightScale = 8.0f;
                float freq = 0.05f;
                float noise = std::sin(worldX * freq) * std::cos(worldZ * freq);

                // Base height + noise height
                int surfaceY = static_cast<int>(32 + (noise * heightScale));

                for (int y = 0; y < CHUNK_SIZE; ++y) {
                    int worldY = chunkPos.y * CHUNK_SIZE + y;

                    Block type = Block::Air;

                    if (worldY < surfaceY - 3) {
                        type = Block::Stone;
                    } else if (worldY < surfaceY) {
                        type = Block::Dirt;
                    } else if (worldY == surfaceY) {
                        type = Block::Grass;
                    }

                    view[x, y, z] = type;
                }
            }
        }
    }

    glm::ivec3 World::worldToChunkPos(const glm::vec3& pos) noexcept {
        // Correctly handle negative coordinates using floor
        // Without floor, -0.5 would become 0, but it should be chunk -1
        return {
            static_cast<int>(std::floor(pos.x / static_cast<float>(CHUNK_SIZE))),
            static_cast<int>(std::floor(pos.y / static_cast<float>(CHUNK_SIZE))),
            static_cast<int>(std::floor(pos.z / static_cast<float>(CHUNK_SIZE)))
        };
    }

    void World::enqueueChunk(const glm::ivec3& pos) {
        if (!m_chunks.contains(pos) && !m_inQueue.contains(pos)) {
            m_loadQueue.push_back(pos);
            m_inQueue.insert(pos);
        }
    }

    void World::processQueue(const glm::vec3& playerPos) {
        if (m_loadQueue.empty()) return;

        // 1. Sort queue so the closest chunk is at the back (O(1) pop)
        // You can do this once every few frames instead of every frame for performance
        std::sort(m_loadQueue.begin(), m_loadQueue.end(), [&](const auto& a, const auto& b) {
            return glm::distance(glm::vec3(a), playerPos / 16.0f) >
                   glm::distance(glm::vec3(b), playerPos / 16.0f);
        });

        // 2. Process exactly one chunk
        glm::ivec3 pos = m_loadQueue.back();
        m_loadQueue.pop_back();
        m_inQueue.erase(pos);

        // This triggers your generation logic
        auto [newIt, inserted] = m_chunks.emplace(pos, Chunk());
        generateChunkData(pos, newIt->second);
    }

}
