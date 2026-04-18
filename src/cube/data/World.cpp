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

        // Lazy Generation: If chunk doesn't exist, create it
        auto [newIt, inserted] = m_chunks.emplace(chunkPos, Chunk());
        generateChunkData(chunkPos, newIt->second);
        return &newIt->second;
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

}
