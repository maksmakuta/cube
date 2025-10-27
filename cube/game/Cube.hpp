#ifndef CUBE_CUBE_HPP
#define CUBE_CUBE_HPP

#include <memory>

#include "graphics/Renderer2D.hpp"
#include "models/Camera.hpp"
#include "render/ChunkRenderer.hpp"
#include "window/Event.hpp"
#include "window/Window.hpp"
#include "utils/ThreadPool.h"

namespace cube {

    class Cube {
    public:
        explicit Cube(const std::shared_ptr<Window>& window);
        ~Cube();

        void onDraw() const;
        void onDrawUI(const Renderer2D&) const;
        void onUpdate(float);
        void onEvent(const Event&);

    private:
        bool is_debug{false};
        float m_fps{0};
        Font m_font;
        Camera m_camera;
        ThreadPool m_pool;
        std::unique_ptr<ChunkRenderer> m_renderer;
        std::unique_ptr<Renderer2D> m_2d_renderer;
        std::shared_ptr<Window> m_window;
    };

}

#endif //CUBE_CUBE_HPP