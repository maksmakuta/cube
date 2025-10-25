#ifndef CUBE_CUBE_HPP
#define CUBE_CUBE_HPP

#include <memory>

#include "Generator.hpp"
#include "graphics/Renderer2D.hpp"
#include "models/Camera.hpp"
#include "render/ChunkRenderer.hpp"
#include "window/Event.hpp"

namespace cube {

    class Cube {
    public:
        Cube();
        ~Cube();

        void onDraw();
        void onDrawUI(const Renderer2D&);
        void onUpdate(float);
        void onEvent(const Event&);

    private:
        bool is_debug = false;
        Camera m_camera;
        World m_world;
        Generator m_gen;
        Font m_font;
        std::unique_ptr<ChunkRenderer> m_renderer{nullptr};
        std::unique_ptr<Renderer2D> m_2drenderer{nullptr};
    };

}

#endif //CUBE_CUBE_HPP