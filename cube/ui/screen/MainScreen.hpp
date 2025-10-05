#ifndef CUBE_MAINSCREEN_HPP
#define CUBE_MAINSCREEN_HPP

#include "core/IScreen.hpp"

namespace cube {

    class MainScreen final : public IScreen{
    public:
        MainScreen();
        ~MainScreen() override;

        void onInit() override;
        void onDeinit() override;
        void onUpdate(float) override;
        void onDraw() override;
        void onTick() override;
        void onEvent(const Event&) override;
    };

}

#endif //CUBE_MAINSCREEN_HPP