#ifndef CUBE_CUBE_HPP
#define CUBE_CUBE_HPP

#include <SDL3/SDL_events.h>

#include "data/Generator.hpp"
#include "data/World.hpp"
#include "graphics/Camera.hpp"
#include "graphics/Renderer.hpp"

namespace cube {

    class Cube final {
    public:
        Cube();

        void onUpdate(float dt);
        void onRender();
        void onEvent(const SDL_Event& event);

    private:
        World m_world;
        Camera m_camera;
        Renderer m_renderer;
        Generator m_generator;
    };

}

#endif //CUBE_CUBE_HPP
