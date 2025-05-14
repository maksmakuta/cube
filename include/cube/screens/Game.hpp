#ifndef GAME_HPP
#define GAME_HPP

#include "cube/core/IScreen.hpp"

namespace cube {

    class Game final : public IScreen {
    public:
        Game();
        ~Game() override;

        void onCreate() override;
        void onClear() override;
        void onDraw() override;
        void onTick() override;

        void onUpdate(float dt) override;
        void onResize(int w, int h) override;
        void onKey(int k, int a, int m) override;
        void onCursor(float x, float y) override;
        void onScroll(float dx, float dy) override;
    };
}


#endif //GAME_HPP
