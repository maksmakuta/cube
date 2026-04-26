#ifndef CUBE_RENDERABLEMESH_HPP
#define CUBE_RENDERABLEMESH_HPP

#include <vector>
#include <glm/vec3.hpp>

#include <cube/graphics/Vertex.hpp>

namespace cube {

    struct RenderableMesh final {
        glm::ivec3 pos;

        std::vector<Vertex> solidVertices;
        std::vector<uint32_t> solidIndices;

        std::vector<Vertex> transVertices;
        std::vector<uint32_t> transIndices;
    };

}

#endif //CUBE_RENDERABLEMESH_HPP
