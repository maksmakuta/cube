#ifndef CUBE_HPP
#define CUBE_HPP

#include <SDL3/SDL_events.h>

namespace cube {

    class Cube final {
    public:
        Cube();

        void onUpdate(float dt);
        void onDraw();
        void onEvent(const SDL_Event& event);
    };

}

#endif //CUBE_HPP
