#ifndef CUBE_HPP
#define CUBE_HPP

#include <SDL3/SDL_events.h>

#include "data/World.hpp"
#include "graphics/Camera.hpp"
#include "graphics/Renderer.hpp"

namespace cube {

    class Cube final {
    public:
        Cube(int argc, char** argv);

        void onUpdate(float dt);
        void onDraw();
        void onEvent(const SDL_Event& event);

    private:
        float m_last_tick;
        glm::vec2 m_view;
        World m_world;
        Camera m_camera;
        Renderer m_renderer;
    };

}

#endif //CUBE_HPP
