#ifndef CUBE_VERTEX_HPP
#define CUBE_VERTEX_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace cube {

    struct Vertex final {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec4 tex;
        glm::vec4 tint;
    };

}

#endif //CUBE_VERTEX_HPP
