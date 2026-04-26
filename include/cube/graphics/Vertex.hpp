#ifndef CUBE_VERTEX_HPP
#define CUBE_VERTEX_HPP

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace cube {

    struct Vertex final {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec4 texture;
        glm::vec4 color;
    };

}

#endif //CUBE_VERTEX_HPP
