#ifndef CUBE_MESH_HPP
#define CUBE_MESH_HPP

#include <vector>

#include <glm/vec3.hpp>

#include "cube/graphics/Vertex.hpp"

namespace cube {

    class World;

    struct MeshData {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };

    struct Mesh final {
        MeshData solid;
        MeshData transparent;
    };

    Mesh generateMesh(const glm::ivec3& pos, const World& world);

}

#endif //CUBE_MESH_HPP
