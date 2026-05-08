#ifndef CUBE_MESHER_HPP
#define CUBE_MESHER_HPP

#include "cube/data/World.hpp"
#include "cube/graphics/Vertex.hpp"

namespace cube {

    struct ChunkMesh {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };

    ChunkMesh mesh(const glm::ivec3& pos, const World& world);

}

#endif //CUBE_MESHER_HPP
