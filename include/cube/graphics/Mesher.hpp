#ifndef MESHER_HPP
#define MESHER_HPP

#include "cube/world/Chunk.hpp"
#include "cube/graphics/Vertex.hpp"
#include "cube/world/World.hpp"

namespace cube {
    struct MeshData {
        std::vector<Vertex3D> vertices;
        std::vector<uint32_t> indices;
    };

    struct VoxelMesh {
        glm::ivec2 key;
        MeshData terrain;
        MeshData billboard;
    };

    class Mesher {
    public:
        Mesher();
        ~Mesher();

        VoxelMesh toMesh(const ChunkPtr &chunk, const std::array<ChunkPtr, 8> &neighbors, const glm::vec2 &chunk_pos);
    };

}

#endif //MESHER_HPP
