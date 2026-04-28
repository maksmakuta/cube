#ifndef CUBE_CUBE_HPP
#define CUBE_CUBE_HPP

#include <SDL3/SDL_events.h>

#include "graphics/Camera.hpp"

namespace cube {

    class Cube {
    public:
        Cube();

        void onUpdate(float dt);
        void onDraw();
        void onEvent(const SDL_Event& event);

    private:
        Camera m_camera;
    };

}

#endif //CUBE_CUBE_HPP
