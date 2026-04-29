#ifndef CUBE_VERTEX_HPP
#define CUBE_VERTEX_HPP

#include <glm/vec3.hpp>

namespace cube {

    struct Vertex final {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 tex;
    };

}

#endif //CUBE_VERTEX_HPP
