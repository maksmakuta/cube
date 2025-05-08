#ifndef CUBE_HPP
#define CUBE_HPP

#include "core/ICallbacks.hpp"

namespace cube {

    class Cube final : public ICallbacks{
    public:
        Cube();

        void onDraw();

        void onResize(int w, int h) override;
        void onKey(int k, int a, int m) override;
        void onCursor(float x, float y) override;
        void onScroll(float dx, float dy) override;
    private:
    };

}

#endif //CUBE_HPP
