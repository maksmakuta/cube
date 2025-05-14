#ifndef ISCREEN_HPP
#define ISCREEN_HPP

#include "cube/core/ICallbacks.hpp"

namespace cube {

    class IScreen : public ICallbacks{
    public:
        ~IScreen() override = default;

        virtual void onCreate() = 0;
        virtual void onClear() = 0;
        virtual void onDraw() = 0;
        virtual void onTick() = 0;

    };

}

#endif //ISCREEN_HPP
