#ifndef MESHER_HPP
#define MESHER_HPP

#include <array>
#include <vector>

#include "cube/graphics/Vertex.hpp"
#include "cube/world/Chunk.hpp"

namespace cube {

    struct Mesh {
        std::vector<Vertex3D> vertices;
        std::vector<uint32_t> indices;
    };

    Mesh toMesh(const ChunkPtr &, const std::array<ChunkPtr,4>& neighbours);
}

#endif //MESHER_HPP
