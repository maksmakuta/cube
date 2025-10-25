#ifndef CUBE_CUBE_HPP
#define CUBE_CUBE_HPP

#include <memory>
#include <unordered_set>

#include "Generator.hpp"
#include "graphics/Renderer2D.hpp"
#include "models/Camera.hpp"
#include "render/ChunkRenderer.hpp"
#include "utils/ThreadPool.hpp"
#include "window/Event.hpp"
#include "window/Window.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace cube {

    class Cube {
    public:
        Cube(const std::shared_ptr<Window>& window);
        ~Cube();

        void onDraw() const;
        void onDrawUI(const Renderer2D&) const;
        void onUpdate(float);
        void onEvent(const Event&);

    private:
        bool is_debug{false};
        float m_fps{0};
        Font m_font;
        World m_world;
        Camera m_camera;
        Generator m_gen;
        std::unordered_set<ChunkPos> m_chunks;
        std::unique_ptr<ChunkRenderer> m_renderer;
        std::unique_ptr<Renderer2D> m_2d_renderer;
        std::shared_ptr<Window> m_window;
    };

}

#endif //CUBE_CUBE_HPP