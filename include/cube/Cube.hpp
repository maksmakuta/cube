#ifndef CUBE_HPP
#define CUBE_HPP

#include <memory>

#include "cube/core/ICallbacks.hpp"
#include "cube/core/IScreen.hpp"

namespace cube {

    class Cube final : public ICallbacks{
    public:
        Cube();

        void onCreate();
        void onClear();
        void onDraw();
        void onTick();

        void onUpdate(float dt) override;
        void onResize(int w, int h) override;
        void onKey(int k, int a, int m) override;
        void onCursor(float x, float y) override;
        void onScroll(float dx, float dy) override;

    private:
        std::unique_ptr<IScreen> m_screen;
        double lastTick = 0;
    };

}

#endif //CUBE_HPP
