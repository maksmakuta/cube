#ifndef CUBE_ISCREEN_HPP
#define CUBE_ISCREEN_HPP

#include "window/Event.hpp"

namespace cube {

    class IScreen {
    public:
        virtual ~IScreen() = default;

        virtual void onInit() = 0;
        virtual void onDeinit() = 0;
        virtual void onUpdate(float) = 0;
        virtual void onDraw() = 0;
        virtual void onTick() = 0;
        virtual void onEvent(const Event&) = 0;

    };

}

#endif //CUBE_ISCREEN_HPP