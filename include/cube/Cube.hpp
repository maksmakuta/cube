#ifndef CUBE_HPP
#define CUBE_HPP

#include "cube/core/ICallbacks.hpp"
#include "cube/entities/Player.hpp"
#include "cube/graphics/renderer/VoxelRenderer.hpp"

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
        Player m_player;
        double lastTick = 0;
        uint8_t m_direction{0};
        bool m_show_debug{false};
    };

}

#endif //CUBE_HPP
