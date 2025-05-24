#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace cube {

    struct Vertex2D {
        glm::i16vec2 pos;
        glm::i16vec2 tex;
        glm::uint col;
    };

    struct Vertex3D {
        glm::vec3 pos;
        glm::vec2 tex;
        int ao;
    };
}

#endif //VERTEX_HPP
