#ifndef CUBE_CUBE_H
#define CUBE_CUBE_H

#include "window/Event.hpp"

namespace cube{

    class Cube {
    public:
        Cube();

        void onInit();
        void onDeinit();
        void onDraw();
        void onTick(float dt);
        void onEvent(const Event& e);
    };

}

#endif //CUBE_CUBE_H
