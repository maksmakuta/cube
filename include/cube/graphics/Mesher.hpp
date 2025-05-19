#ifndef MESHER_HPP
#define MESHER_HPP

#include "cube/world/Chunk.hpp"
#include "cube/graphics/Vertex.hpp"
#include "cube/world/World.hpp"

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

        VoxelMesh toMesh(World& w, const glm::ivec2& current);

    };

}

#endif //MESHER_HPP
