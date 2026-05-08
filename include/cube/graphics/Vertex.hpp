#ifndef CUBE_VERTEX_HPP
#define CUBE_VERTEX_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace cube {

    struct Vertex final {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texture_uv;
        float texture_id;
    };

}

#endif //CUBE_VERTEX_HPP
