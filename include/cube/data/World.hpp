#ifndef CUBE_WORLD_HPP
#define CUBE_WORLD_HPP

#include <memory>
#include <unordered_map>

#include "cube/core/Status.hpp"
#include "cube/data/Chunk.hpp"
#include "cube/graphics/Renderer.hpp"

namespace cube {

    class World final {
    public:
        World();

        Status getStatus(const glm::ivec3&);
        void setStatus(const glm::ivec3&,Status);

        std::shared_ptr<Chunk> getChunk(const glm::ivec3&);
        void setChunk(const glm::ivec3&,std::shared_ptr<Chunk>);

    private:
        std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_chunks;
        std::unordered_map<glm::ivec3, Status> m_statuses;
    };

}

#endif //CUBE_WORLD_HPP
