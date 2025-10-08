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
        void onDraw(Context&) override;
        void onTick(float) override;
        void onEvent(const Event&) override;
    private:
        void onDraw2D(Renderer2D&);
        void loadTexture();

        Texture dirt;
        int id = 0;
    };

}

#endif //CUBE_MAINSCREEN_HPP