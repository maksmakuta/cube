#ifndef CUBE_ISCREEN_HPP
#define CUBE_ISCREEN_HPP

#include "graphics/Context.hpp"
#include "window/Event.hpp"

namespace cube {

    class IScreen {
    public:
        virtual ~IScreen() = default;

        virtual void onInit() = 0;
        virtual void onDeinit() = 0;
        virtual void onTick(float) = 0;
        virtual void onDraw(Context&) = 0;
        virtual void onEvent(const Event&) = 0;
    };

}

#endif //CUBE_ISCREEN_HPP