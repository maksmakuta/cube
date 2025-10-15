#ifndef CUBE_WORLD_HPP
#define CUBE_WORLD_HPP

#include <memory>
#include <unordered_map>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "Chunk.hpp"

namespace cube {

    using ChunkPos = glm::ivec2;
    using ChunkPtr = std::shared_ptr<Chunk>;

    class World {
    public:
        World();
        ~World();

        ChunkPtr getChunk(const ChunkPos&);
        void setChunk(const ChunkPos&, ChunkPtr);
        void removeChunk(const ChunkPos&);

    private:
        std::unordered_map<ChunkPos, ChunkPtr> m_chunks;
    };

}

#endif //CUBE_WORLD_HPP