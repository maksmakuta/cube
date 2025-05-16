#ifndef GAME_HPP
#define GAME_HPP

#include "cube/core/IScreen.hpp"
#include "cube/entities/Player.hpp"
#include "cube/graphics/ChunkRenderer.hpp"
#include "cube/graphics/Renderer.hpp"
#include "cube/utils/ThreadPool.hpp"
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
        ThreadPool m_pool;
        ChunkRenderer m_chunker;
        Renderer m_renderer;
        Font m_font{24};
        Player m_player;
        World m_world;

        float m_last{0.f};
        float m_fps{0.f};
        uint m_frames{0};

        uint8_t m_player_dir{0};
        bool m_debug{false};
    };
}


#endif //GAME_HPP
