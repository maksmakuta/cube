#ifndef CUBE_WORLD_HPP
#define CUBE_WORLD_HPP

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <unordered_map>
#include <functional>
#include <memory>
#include <shared_mutex>
#include <unordered_set>

#include <cube/data/Chunk.hpp>
#include <cube/data/Generator.hpp>

#include "Mesh.hpp"
#include "cube/graphics/Renderer.hpp"
#include "cube/utils/ResultQueue.hpp"
#include "cube/utils/ThreadPool.hpp"

namespace cube {

    constexpr int RENDER_DISTANCE = 16;

    using UnloadCallback = std::function<void(const glm::ivec3 &)>;

    struct ChunkResult {
        glm::ivec3 pos;
        Chunk chunk;
    };

    class World {
    public:
        explicit World(int seed);

        std::shared_ptr<Chunk> getChunk(const glm::ivec3& chunkPos);
        Block getBlock(const glm::ivec3& worldPos);

        ChunkNeighbors getNeighbors(const glm::ivec3 & pos);

        std::vector<glm::ivec3> loadArea(const glm::ivec3& centerChunk, int radius);
        void unloadFarChunks(const glm::ivec3& centerChunk, int radius, const UnloadCallback& onUnload);

        void processAsyncResults(Renderer &renderer);

    private:
        void ensureChunkExists(const glm::ivec3& pos);

        std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_chunks;
        std::shared_mutex m_mapMutex;
        Generator m_generator;

        ThreadPool m_threadPool{4};

        std::unordered_set<glm::ivec3> m_generating;

        ResultQueue<ChunkResult> m_chunkQueue;
        ResultQueue<RenderableMesh> m_meshQueue;
    };

}

#endif //CUBE_WORLD_HPP
