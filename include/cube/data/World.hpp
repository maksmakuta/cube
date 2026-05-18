#ifndef CUBE_WORLD_HPP
#define CUBE_WORLD_HPP

#include <memory>
#include <unordered_map>
#include <shared_mutex>

#include "cube/core/Status.hpp"
#include "cube/data/Chunk.hpp"
#include "cube/graphics/Renderer.hpp"

#define  GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace cube {

    class World final {
    public:
        World();

        Status getStatus(const glm::ivec3&);
        void setStatus(const glm::ivec3&,Status);

        std::shared_ptr<Chunk> getChunk(const glm::ivec3&) const;
        void setChunk(const glm::ivec3&,std::shared_ptr<Chunk>);

        void unloadChunks(const glm::ivec3& pos, int render_distance);
        bool readyToMesh(const glm::ivec3& pos) const;

    private:
        mutable std::shared_mutex m_worldMutex;
        std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_chunks;
        std::unordered_map<glm::ivec3, Status> m_statuses;
    };

}

#endif //CUBE_WORLD_HPP
