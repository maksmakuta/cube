#ifndef CUBE_HPP
#define CUBE_HPP

#include "cube/core/ICallbacks.hpp"
#include "cube/entities/Player.hpp"
#include "cube/graphics/renderer/VoxelRenderer.hpp"
#include "graphics/renderer/Renderer.hpp"

namespace cube {

    class Cube final : public ICallbacks{
    public:
        Cube();

        void onCreate();
        void onClear();
        void onDraw();
        void onTick();

        void onRender(Renderer& r);
        void onRender(VoxelRenderer& r);

        void onUpdate(float dt) override;
        void onResize(int w, int h) override;
        void onKey(int k, int a, int m) override;
        void onCursor(float x, float y) override;
        void onScroll(float dx, float dy) override;

    private:
        Renderer m_renderer;
        VoxelRenderer m_voxel;
        Font m_font{24};
        Player m_player;
        double lastTick = 0;
        uint8_t m_direction{0};
        bool m_show_debug{false};
        glm::vec2 mouse{};
    };

}

#endif //CUBE_HPP
