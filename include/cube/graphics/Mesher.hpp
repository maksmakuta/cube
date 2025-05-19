#ifndef MESHER_HPP
#define MESHER_HPP

#include "cube/world/Chunk.hpp"
#include "cube/graphics/Vertex.hpp"

namespace cube {

    struct VoxelMesh {
        glm::ivec2 key;
        std::vector<Vertex3D> vertices;
        std::vector<uint32_t> indices;
    };

    class Mesher {
    public:
        Mesher();
        ~Mesher();

        VoxelMesh toMesh(const Chunk& c);

    };

}

#endif //MESHER_HPP
