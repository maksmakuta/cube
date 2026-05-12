#ifndef CUBE_WORLD_HPP
#define CUBE_WORLD_HPP

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <unordered_map>
#include <memory>
#include <shared_mutex>

#include "Chunk.hpp"
#include "cube/core/ChunkStatus.hpp"

namespace cube {

    using ChunkPtr = std::shared_ptr<Chunk>;

    class World final {
    public:
        World();

        bool contains(const glm::ivec3& pos) const;
        ChunkStatus getStatus(const glm::ivec3& pos) const;
        ChunkPtr getChunk(const glm::ivec3& pos) const;

        void setStatus(const glm::ivec3& pos,ChunkStatus);
        void setChunk(const glm::ivec3& pos,ChunkPtr);

        int clearChunks(const glm::ivec3& pos, int dist);

    private:
        mutable std::shared_mutex m_mutex;
        std::unordered_map<glm::ivec3, ChunkStatus> m_statuses;
        std::unordered_map<glm::ivec3, ChunkPtr> m_chunks;
    };

}

#endif //CUBE_WORLD_HPP
