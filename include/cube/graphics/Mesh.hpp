#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include "cube/world/Chunk.hpp"
#include "cube/graphics/renderer/Vertex.hpp"

namespace cube {

    using ChunkMesh = std::vector<Vertex3D>;

    class Mesh {
    public:
        Mesh();
        ~Mesh();

        static ChunkMesh toMesh(Chunk&);
    };

}

#endif //MESH_HPP
