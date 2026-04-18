#ifndef CUBE_WORLD_HPP
#define CUBE_WORLD_HPP
#pragma once

#include <unordered_map>
#include <glm/vec3.hpp>
#include <deque>
#include <unordered_set>

// Required for hashing glm::ivec3 in unordered_map
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "Chunk.hpp"

namespace cube {

    class World final {
    public:
        explicit World(int seed = 0);

        // Disable copying - prevent the "Unique_ptr/Heavy object copy" error
        World(const World&) = delete;
        World& operator=(const World&) = delete;

        // Use pointers for stable memory addresses (crucial for voxel engines)
        Chunk* getChunk(const glm::ivec3& chunkPos);

        // Pass by value and move into the map
        bool putChunk(const glm::ivec3& chunkPos, Chunk&& chunk);

        void unloadDistantChunks(const glm::ivec3& playerChunkPos, int viewDistance);

        // Return a const reference to avoid massive copies
        [[nodiscard]] const std::unordered_map<glm::ivec3, Chunk>& getChunks() const noexcept {
            return m_chunks;
        }

        // Helper to convert world float position to chunk integer position
        static glm::ivec3 worldToChunkPos(const glm::vec3& pos) noexcept;

        void enqueueChunk(const glm::ivec3& pos);
        void processQueue(const glm::vec3& playerPos);

    private:
        static void generateChunkData(const glm::ivec3& chunkPos, Chunk& chunk);

        int m_seed;
        // Storing by value is okay ONLY if Chunk is move-friendly and you return by reference
        std::unordered_map<glm::ivec3, Chunk> m_chunks;
        std::deque<glm::ivec3> m_loadQueue;
        std::unordered_set<glm::ivec3> m_inQueue;
    };

}

#endif //CUBE_WORLD_HPP
