#ifndef CUBE_MESHER_HPP
#define CUBE_MESHER_HPP

#include <vector>
#include <glm/vec3.hpp>

#include "cube/graphics/Vertex.hpp"

namespace cube {

    class World;

    struct ChunkMesh {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };

    ChunkMesh mesh(const glm::ivec3& pos, const World& world);

}

#endif //CUBE_MESHER_HPP
