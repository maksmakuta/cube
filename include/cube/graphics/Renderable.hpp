#ifndef CUBE_RENDERABLE_HPP
#define CUBE_RENDERABLE_HPP

#include <glm/common.hpp>

namespace cube {

    struct Renderable {
        glm::uint vao;
        glm::uint vbo;
        glm::uint ebo;
        glm::uint indices;
    };

}

#endif //CUBE_RENDERABLE_HPP
