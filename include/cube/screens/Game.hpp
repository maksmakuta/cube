#ifndef GAME_HPP
#define GAME_HPP

#include "cube/core/IScreen.hpp"
#include "cube/entities/Player.hpp"
#include "cube/world/World.hpp"

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
        void onText(uint) override;

    private:
        World m_world;
        Player m_player;
        uint8_t m_player_dir{0};
    };
}


#endif //GAME_HPP
