#ifndef IRENDERABLE_HPP
#define IRENDERABLE_HPP

#include <glm/common.hpp>

namespace cube {

    struct IRenderable {
        glm::uint id;
        glm::uint vao;
        glm::uint vbo;
        glm::uint ebo;
        int count;
    };

}

#endif //IRENDERABLE_HPP
