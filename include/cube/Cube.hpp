#ifndef CUBE_CUBE_HPP
#define CUBE_CUBE_HPP

#include <SDL3/SDL_events.h>

namespace cube {

    class Cube final {
    public:
        Cube();
        ~Cube();

        void onDraw();
        void onUpdate(float dt);
        void onEvent(const SDL_Event&);
    };

}

#endif //CUBE_CUBE_HPP
