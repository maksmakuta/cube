#ifndef CUBE_HPP
#define CUBE_HPP

#include "core/ICallbacks.hpp"
#include "graphics/renderer/Renderer.hpp"
#include "graphics/renderer/VoxelRenderer.hpp"
#include "world/World.hpp"

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
        Renderer m_renderer;
        VoxelRenderer m_voxel;
        World m_world;
    };

}

#endif //CUBE_HPP
