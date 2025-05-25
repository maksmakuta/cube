#ifndef CUBE_HPP
#define CUBE_HPP

#include "core/IWindowController.hpp"
#include "entities/Player.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/VoxelRenderer.hpp"
#include "utils/ThreadPool.hpp"
#include "world/World.hpp"

namespace cube {
    class Cube final {
    public:
        Cube();

        void onCreate();
        void onClear();
        void onDraw();

        void onTick();

        void onUpdate(float dt);
        void onResize(int w, int h);
        void onKey(int k, int a, int m);
        void onCursor(float x, float y);
        void onScroll(float dx, float dy);
        void onText(uint code);

        void showCursor(bool);
        void setCursor(CursorIcon);
        void close();

        void attachController(IWindowController*);

    private:
        ThreadPool m_pool;
        VoxelRenderer m_voxel;
        Renderer m_renderer;
        Font m_font{24};
        World m_world;
        Player m_player;
        double lastTick = 0;
        float m_last{0.f};
        float m_fps{0.f};
        float m_speed{1.f};
        uint m_frames{0};
        uint8_t m_player_dir{0};
        bool m_debug{false};
        IWindowController* m_win_controller = nullptr;
    };

}

#endif //CUBE_HPP
