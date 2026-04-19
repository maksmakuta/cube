#ifndef CUBE_CUBE_HPP
#define CUBE_CUBE_HPP

#include <SDL3/SDL_events.h>

#include "graphics/Camera.hpp"
#include "window/Window.hpp"

namespace cube {

    class Cube {
    public:
        Cube();

        void onDraw();
        void onUpdate(float dt);
        void onResize(int w, int h);
        void onEvent(const SDL_Event &event);

    private:
        Camera m_camera;
        glm::mat4 m_projection{1.f};
    };

}

#endif //CUBE_CUBE_HPP
