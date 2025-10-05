#ifndef CUBE_ISCREEN_HPP
#define CUBE_ISCREEN_HPP

namespace cube {

    class IScreen {
    public:
        virtual ~IScreen() = default;

        virtual void onInit() = 0;
        virtual void onDeinit() = 0;
        virtual void onUpdate(float) = 0;
        virtual void onDraw() = 0;
        virtual void onTick() = 0;
        virtual void onEvent() = 0;

    };

}

#endif //CUBE_ISCREEN_HPP