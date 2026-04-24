#ifndef CUBE_RENDERABLEMESH_HPP
#define CUBE_RENDERABLEMESH_HPP

#include <vector>
#include <glm/vec3.hpp>

#include <cube/graphics/Vertex.hpp>

namespace cube {

    struct RenderableMesh final {
        glm::ivec3 pos;
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };

}

#endif //CUBE_RENDERABLEMESH_HPP
