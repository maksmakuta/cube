#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace cube {

    struct Vertex2D {
        glm::vec2 pos;
        glm::vec2 tex;
        glm::vec4 col;
    };

    struct Vertex3D {
        glm::vec3 pos;
        glm::vec2 tex;
    };
}

#endif //VERTEX_HPP
