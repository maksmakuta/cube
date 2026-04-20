#ifndef CUBE_MESHING_HPP
#define CUBE_MESHING_HPP

#include <glm/vec3.hpp>

#include "cube/data/World.hpp"
#include "cube/graphics/Vertex.hpp"

namespace cube {

    struct MeshResult {
        glm::ivec3 pos;
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };

    struct MeshTask {
        glm::ivec3 pos;
        int priority;
    };

    static MeshResult mesh(World&, MeshTask);

}

#endif //CUBE_MESHING_HPP
