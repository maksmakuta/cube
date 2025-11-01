#ifndef CUBE_CUBE_H
#define CUBE_CUBE_H

#include "window/Event.hpp"
#include "graphics/Font.hpp"
#include "graphics/Renderer2D.hpp"

namespace cube{

    class Cube {
    public:
        Cube();

        void onInit();
        void onDeinit();
        void onDraw();
        void onTick(float dt);
        void onEvent(const Event& e);

    private:
        void onRender(Renderer2D&);

        float m_fps{0};
        Font m_font;
        glm::vec2 mouse{0};
        std::unique_ptr<Renderer2D> m_renderer;
    };

}

#endif //CUBE_CUBE_H
