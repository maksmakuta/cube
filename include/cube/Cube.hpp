#ifndef CUBE_HPP
#define CUBE_HPP

#include <memory>

#include "core/IController.hpp"
#include "cube/core/IScreen.hpp"

namespace cube {

    class Cube final : public IController{
    public:
        Cube();

        void onCreate();
        void onClear();
        void onDraw();

        void onUpdate(float dt);
        void onResize(int w, int h);
        void onKey(int k, int a, int m);
        void onCursor(float x, float y);
        void onScroll(float dx, float dy);
        void onText(uint code);

        void showCursor(bool) override;
        void setCursor(CursorIcon) override;
        void navigate(IScreen *) override;
        void close() override;

        void attachController(IWindowController*);

    private:
        std::unique_ptr<IScreen> m_screen;
        IWindowController* m_win_controller = nullptr;
        double lastTick = 0;
    };

}

#endif //CUBE_HPP
