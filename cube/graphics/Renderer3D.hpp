#ifndef CUBE_RENDERER3D_HPP
#define CUBE_RENDERER3D_HPP

#include "RendererState.hpp"

namespace cube {

    class Renderer3D {
    public:
        Renderer3D();
        ~Renderer3D();

    private:
        RendererState3D m_state;
    };

}

#endif //CUBE_RENDERER3D_HPP