#ifndef CUBE_WORLD_HPP
#define CUBE_WORLD_HPP

#include <memory>
#include <unordered_map>
#include <glm/vec2.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "Chunk.hpp"

namespace cube {

    using ChunkPos = glm::ivec2;

    class World {
    public:
        World();
        ~World();

        ChunkPtr getChunk(const ChunkPos&);
        void setChunk(const ChunkPos&, ChunkPtr);
        void removeChunk(const ChunkPos&);

        void forEachChunk(const std::function<void(const ChunkPos&, const ChunkPtr&)>&);

    private:
        std::unordered_map<ChunkPos, ChunkPtr> m_chunks;
    };

}

#endif //CUBE_WORLD_HPP