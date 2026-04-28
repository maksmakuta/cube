#ifndef CUBE_WORLD_HPP
#define CUBE_WORLD_HPP

#include <shared_mutex>
#include <memory>
#include <mutex>
#include <unordered_map>

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "cube/data/Chunk.hpp"

namespace cube {

    class World final {
    public:
        World() = default;

        void insertChunk(const glm::ivec3& pos, std::unique_ptr<Chunk> chunk);
        std::vector<glm::ivec3> processPendingChunks();
        std::shared_ptr<const Chunk> getChunkReadonly(const glm::ivec3& pos) const;

    private:
        struct PendingChunk { glm::ivec3 pos; std::unique_ptr<Chunk> chunk; };

        std::mutex m_uploadMutex;
        std::vector<PendingChunk> m_pendingChunks;

        mutable std::shared_mutex m_worldMutex;
        std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_chunks;
    };

}

#endif //CUBE_WORLD_HPP
