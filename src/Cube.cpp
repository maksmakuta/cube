#include "cube/Cube.hpp"

#include <format>
#include <iostream>
#include <GLFW/glfw3.h>

#include "cube/core/Constants.hpp"

namespace cube {

    Cube::Cube() = default;

    void Cube::onCreate() {
        showCursor(false);
        m_renderer.onCreate();
        m_voxel.onCreate();
        m_font.load(getAsset("/fonts/BlockCraft.otf"));
        // m_player.setPosition({0, 96, 0});
    }

    void Cube::onClear() {
        m_voxel.onClear();
        m_pool.unload();
        m_renderer.onClear();
        m_font.unload();
    }

    void Cube::onDraw() {
        clear(0xFF222222);

        m_voxel.onDraw(m_player.getCamera().getView());

        if (m_debug) {
            const auto h = m_font.getSize();
            const auto pos = m_player.getPosition();
            const auto rot = m_player.getRotation();
            const auto cnk = toChunk(pos);

            m_renderer.text(m_font, 0xFFFFFFFF);
            m_renderer.print({0, h}, std::format("Cube v. 0.8.0 [debug] FPS: {:.2f}", m_fps));
            m_renderer.print({0, h * 2}, std::format("Position: [{:.2f},{:.2f},{:.2f}]", pos.x, pos.y, pos.z));
            m_renderer.print({0, h * 3}, std::format("Rotation: [{:.2f},{:.2f}]", rot.x, rot.y));
            m_renderer.print({0, h * 4}, std::format("Speed: {:.2f}", m_speed));

            m_renderer.print({0, h * 5}, std::format("Chunk: [{},{}]", cnk.x, cnk.y));
            m_renderer.print({0, h * 6}, std::format("Render Distance: {}", RENDER_DIST));
            m_renderer.print({0, h * 7}, std::format("Chunk size: [{},{},{}]", CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH));

            m_renderer.print({0, h * 9}, std::format("Loaded: {}", m_world.getChunks().size()));
            m_renderer.print({0, h * 10}, std::format("Visible: {}", m_world.getVisibleChunks().size()));
        }
    }

    void Cube::onTick() {
        m_world.onTick(m_pool, m_player);
        m_voxel.onTick(m_pool, m_world);
    }

    void Cube::onUpdate(const float dt) {
        m_frames++;
        m_last += dt;

        if (m_last > FPS_TICK) {
            m_fps = static_cast<float>(m_frames) / m_last;
            constexpr float alpha = 0.1f;
            m_fps = (1.0f / dt) * alpha + m_fps * (1.0f - alpha);
            m_frames = 0;
            m_last = 0;
        }

        m_player.move(m_player_dir, dt * m_speed);

        lastTick += dt;
        if (lastTick > TICK) {
            onTick();
            lastTick = 0;
        }
    }

    void Cube::onResize(const int w, const int h) {
        glViewport(0, 0, w, h);
        m_renderer.onResize(w,h);
        m_voxel.onResize(w, h);
    }

    void Cube::onKey(const int k, const int a, const int m) {
        if (a != GLFW_RELEASE) {
            if (k == GLFW_KEY_LEFT_CONTROL || m == GLFW_MOD_CONTROL) {
                m_speed = 2.0f;
            } else {
                m_speed = 1.f;
            }
        }

        const auto keys = std::vector{
            GLFW_KEY_W,
            GLFW_KEY_S,
            GLFW_KEY_A,
            GLFW_KEY_D,
            GLFW_KEY_SPACE,
            GLFW_KEY_LEFT_SHIFT
        };

        for (int i = 0; i < keys.size(); i++) {
            if (k == keys[i]) {
                set(m_player_dir, 1 << (i + 1), a != GLFW_RELEASE);
            }
        }

        if (k == GLFW_KEY_F1 && a == GLFW_PRESS) {
            m_debug = !m_debug;
        }
    }

    void Cube::onCursor(const float x, const float y){
        m_player.rotate(x,y);
    }

    void Cube::onScroll(const float dx, const float dy){

    }

    void Cube::onText(uint code) {

    }

    void onController(IWindowController* c,const std::function<void(IWindowController*)>& fn) {
        if (c){
            fn(c);
        }else {
            std::cerr << "No window controllers attached!" << std::endl;
        }
    }

    void Cube::showCursor(bool b) {
        onController(m_win_controller,[&b](IWindowController* c){
            c->showCursor(b);
        });
    }

    void Cube::setCursor(CursorIcon i) {
        onController(m_win_controller,[&i](IWindowController* c){
            c->setCursor(i);
        });
    }

    void Cube::close() {
        onController(m_win_controller,[](IWindowController* c){
            c->close();
        });
    }

    void Cube::attachController(IWindowController* c) {
        m_win_controller = c;
    }

}

