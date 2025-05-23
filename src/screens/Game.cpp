#include "cube/screens/Game.hpp"
#include "glad/gl.h"

#include <format>
#include <random>
#include <iostream>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "cube/generators/GeneratorTerrain.hpp"
#include "cube/utils/Utils.hpp"

namespace cube {

    Game::Game() : IScreen(){}
    Game::~Game() = default;

    void Game::onCreate() {
        controller()->showCursor(false);
        m_voxel.onCreate();
        m_renderer.onCreate();
        m_font.load(getAsset("/fonts/BlockCraft.otf"));
        m_player.setPosition({0,96,0});
    }

    void Game::onClear() {
        m_pool.unload();
        m_voxel.onClear();
        m_renderer.onClear();
        m_font.unload();
    }

    void Game::onDraw() {
        clear(0xFF222222);

        m_voxel.onDraw(m_player.getCamera().getView(), m_player.getPosition());

        if (m_debug) {
            const auto h = m_font.getSize();
            const auto pos = m_player.getPosition();
            const auto rot = m_player.getRotation();
            const auto cnk = toChunk(pos);

            m_renderer.text(m_font,0xFFFFFFFF);
            m_renderer.print({0,h},std::format("Cube v. 0.7.x [debug] FPS: {:.2f}", m_fps));
            m_renderer.print({0,h*2},std::format("Position: [{:.2f},{:.2f},{:.2f}]", pos.x,pos.y,pos.z));
            m_renderer.print({0,h*3},std::format("Rotation: [{:.2f},{:.2f}]", rot.x,rot.y));
            m_renderer.print({0,h*4},std::format("Chunk: [{},{}]", cnk.x, cnk.y));

            m_renderer.print({0,h*6},std::format("Render Distance: {}", RENDER_DIST));
            m_renderer.print({0,h*7},std::format("Chunk size: [{},{},{}]", CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH));

            m_renderer.print({0,h*9},std::format("Seed: {}", m_world.getSeed()));
        }
    }

    void Game::onTick() {
        m_world.onTick(m_pool,m_player.getPosition());
        m_voxel.onTick(m_pool,m_world);
    }

    void Game::onUpdate(const float dt) {
        m_frames++;
        m_last += dt;

        if (m_last > FPS_TICK) {
            m_fps = static_cast<float>(m_frames) / m_last;
            constexpr float alpha = 0.1f;
            m_fps = (1.0f / dt) * alpha + m_fps * (1.0f - alpha);
            m_frames = 0;
            m_last = 0;
        }

        m_player.move(m_player_dir,dt * m_speed);
    }

    void Game::onResize(const int w, const int h) {
        m_renderer.onResize(w,h);
        m_voxel.onResize(w,h);
    }

    void Game::onKey(const int k, const int a, const int m) {
        const auto keys = std::vector{
            GLFW_KEY_W,
            GLFW_KEY_S,
            GLFW_KEY_A,
            GLFW_KEY_D,
            GLFW_KEY_SPACE,
            GLFW_KEY_LEFT_SHIFT
        };

        for(int i = 0; i < keys.size();i++) {
            if (k == keys[i]) {
                if (i == 0) {
                    if (m == GLFW_MOD_CONTROL) {
                        m_speed = 2.0f;
                    }else if (m == GLFW_MOD_SHIFT) {
                        m_speed = 0.5f;
                    }else {
                        m_speed = 1.f;
                    }
                }
                set(m_player_dir,1 << (i+1),a != GLFW_RELEASE);
            }
        }

        if (k == GLFW_KEY_F1 && a == GLFW_PRESS){
            m_debug = !m_debug;
        }
    }

    void Game::onCursor(const float x, const float y) {
        m_player.rotate(x,y);
    }

    void Game::onScroll(float dx, float dy){}
    void Game::onText(uint) {}
    
}
