#ifndef CUBE_SCREENMANAGER_HPP
#define CUBE_SCREENMANAGER_HPP

#include "window/Event.hpp"

namespace cube {

    class ScreenManager {
    public:
        ScreenManager();
        ~ScreenManager();

        void onInit();
        void onDeinit();
        void onUpdate(float);
        void onDraw();
        void onTick();
        void onEvent(const Event&);
    };

}

#endif //CUBE_SCREENMANAGER_HPP