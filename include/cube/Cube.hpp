#ifndef CUBE_HPP
#define CUBE_HPP

#include <SDL3/SDL_events.h>

#include "data/World.hpp"

namespace cube {

    class Cube final {
    public:
        Cube(int argc, char** argv);

        void onUpdate(float dt);
        void onDraw();
        void onEvent(const SDL_Event& event);

    private:
        float m_last_tick;
        World m_world;
    };

}

#endif //CUBE_HPP
