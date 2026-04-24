#ifndef CUBE_WORLD_HPP
#define CUBE_WORLD_HPP

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <unordered_map>
#include <functional>
#include <memory>
#include <shared_mutex>

#include <cube/data/Chunk.hpp>
#include <cube/data/Generator.hpp>

#include "Mesh.hpp"

namespace cube {

    using UnloadCallback = std::function<void(const glm::ivec3 &)>;

    class World {
    public:
        explicit World(int seed);

        Chunk* getChunk(const glm::ivec3& chunkPos);
        Block getBlock(const glm::ivec3& worldPos);
        ChunkNeighbors getNeighbors(const glm::ivec3 & pos);
        std::vector<glm::ivec3> loadArea(const glm::ivec3& centerChunk, int radius);

        void unloadFarChunks(const glm::ivec3& centerChunk, int radius, const UnloadCallback& onUnload);

    private:
        void ensureChunkExists(const glm::ivec3& pos);

        std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>> m_chunks;
        std::shared_mutex m_mapMutex;
        Generator m_generator;
    };

}

#endif //CUBE_WORLD_HPP
