#ifndef CUBE_RENDERERUI_HPP
#define CUBE_RENDERERUI_HPP

#include <memory>

#include "Renderer2D.hpp"

namespace cube {

    class RendererUI {
    public:
        RendererUI();
        ~RendererUI();



    private:
        std::shared_ptr<Renderer2D> m_2d_renderer;
    };
}

#endif //CUBE_RENDERERUI_HPP