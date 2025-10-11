#ifndef CUBE_RENDERERUI_HPP
#define CUBE_RENDERERUI_HPP

#include <memory>

#include "Renderer2D.hpp"
#include "ui/Rect.hpp"

namespace cube {

    class RendererUI {
    public:
        RendererUI();
        ~RendererUI();

        void drawIcon(const Rect& rect, int id);
        void drawText(const glm::vec2& pos, const std::string& text);
        void drawBackground(const Rect& rect, const glm::vec4& color);

    private:
        Font m_font;
        std::shared_ptr<Renderer2D> m_2d_renderer;
    };
}

#endif //CUBE_RENDERERUI_HPP