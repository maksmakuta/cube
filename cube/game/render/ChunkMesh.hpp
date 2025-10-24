#ifndef CUBE_CHUNKMESH_HPP
#define CUBE_CHUNKMESH_HPP

#include <cstdint>
#include <vector>

#include "ChunkVertex.hpp"
#include "models/world/Chunk.hpp"

namespace cube {

    struct ChunkMesh {
        std::vector<ChunkVertex> vertices;
        std::vector<uint32_t> indices;

        void reserve(size_t v, size_t i);
        void clear();
    };

    ChunkMesh toMesh(const ChunkPtr&, const std::vector<ChunkPtr>&);

}

#endif //CUBE_CHUNKMESH_HPP