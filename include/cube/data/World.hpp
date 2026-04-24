#ifndef CUBE_WORLD_HPP
#define CUBE_WORLD_HPP

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <unordered_map>
#include <shared_mutex>

#include <cube/data/Chunk.hpp>
#include <cube/data/Generator.hpp>

namespace cube {

    class World {
    public:
        explicit World(int seed);

        Chunk* getChunk(const glm::ivec3& chunkPos);
        void loadArea(const glm::ivec3& centerChunk, int radius);
        Block getBlock(const glm::ivec3& worldPos);

    private:
        void ensureChunkExists(const glm::ivec3& pos);

        std::unordered_map<glm::ivec3, Chunk> m_chunks;
        std::shared_mutex m_mapMutex;
        Generator m_generator;
    };

}

#endif //CUBE_WORLD_HPP
