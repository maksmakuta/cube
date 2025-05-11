#ifndef CUBE_HPP
#define CUBE_HPP

#include "cube/core/ICallbacks.hpp"
#include "cube/entities/Player.hpp"
#include "cube/graphics/renderer/Renderer.hpp"
#include "cube/graphics/renderer/VoxelRenderer.hpp"
#include "cube/world/World.hpp"

#include "BS_thread_pool.hpp"

namespace cube {

    class Cube final : public ICallbacks{
    public:
        Cube();

        void onCreate();
        void onClear();
        void onDraw();

        void onUpdate(float dt) override;
        void onResize(int w, int h) override;
        void onKey(int k, int a, int m) override;
        void onCursor(float x, float y) override;
        void onScroll(float dx, float dy) override;

    private:
        BS::thread_pool<> pool;
        Renderer m_renderer;
        VoxelRenderer m_voxel;
        World m_world;
        Player m_player;
        uint m_direction{0};
        bool m_move_player{false};
    };

}

#endif //CUBE_HPP
