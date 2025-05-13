#include "cube/Cube.hpp"

#include <iostream>
#include <format>
#include <GLFW/glfw3.h>

#include "cube/core/Constants.hpp"
#include "cube/graphics/renderer/Renderer.hpp"

namespace cube {

    Cube::Cube() = default;

    void Cube::onCreate() {
        m_voxel.onCreate();
        m_renderer.onCreate();
        m_font.load(getAsset("/fonts/BlockCraft.otf"));
    }

    void Cube::onClear() {
        m_voxel.onClear();
        m_renderer.onClear();
        m_font.unload();
    }

    void Cube::onDraw() {
        clear(0xFF202020);
        m_voxel.use();
        onRender(m_voxel);
        m_renderer.use();
        onRender(m_renderer);
    }

    void Cube::onRender(Renderer& r) const {
        if (m_show_debug) {
            const auto h = static_cast<float>(m_font.getSize()) * 1.0f;
            const auto pos = m_player.getPosition();
            const auto rot = m_player.getRotation();
            const auto chk = toChunk(pos);

            r.text(m_font,0xFFFFFFFF);
            r.print({0,h*1},std::format("Position: [{:.2f},{:.2f},{:.2f}]",pos.x,pos.y,pos.z));
            r.print({0,h*2},std::format("Rotation: [{:.2f},{:.2f}]",rot.x,rot.y));
            r.print({0,h*3},std::format("Chunk: [{},{}]",chk.x,chk.y));
        }

        r.fill(0xFF00FF00);
        r.circle(m.x,m.y,50);
    }

    void Cube::onRender(VoxelRenderer& r) const{

    }

    void Cube::onTick() {
        m_world.onTick(m_player.getPosition());
    }

    void Cube::onUpdate(const float dt) {
        lastTick += dt;
        if (lastTick > TICK) {
            onTick();
            lastTick = 0;
        }
        m_player.move(m_direction,dt);
    }

    void Cube::onResize(const int w, const int h){
        glViewport(0,0,w,h);
        m_renderer.onResize(w,h);
        m_voxel.onResize(w,h);
    }

    void Cube::onKey(const int k, const int a, const int m){
        if (k == GLFW_KEY_F1 && a == GLFW_PRESS) {
            m_show_debug = !m_show_debug;
        }
        if (k == GLFW_KEY_W) {
            set(m_direction,Forward,a != GLFW_RELEASE);
        }
        if (k == GLFW_KEY_S) {
            set(m_direction,Backward,a != GLFW_RELEASE);
        }
        if (k == GLFW_KEY_A) {
            set(m_direction,Left,a != GLFW_RELEASE);
        }
        if (k == GLFW_KEY_D) {
            set(m_direction,Right,a != GLFW_RELEASE);
        }
        if (k == GLFW_KEY_SPACE) {
            set(m_direction,Up,a != GLFW_RELEASE);
        }
        if (k == GLFW_KEY_LEFT_SHIFT) {
            set(m_direction,Down,a != GLFW_RELEASE);
        }
    }

    void Cube::onCursor(const float x, const float y){
        m_player.rotate(x,y);
        m = {x,y};
    }

    void Cube::onScroll(const float dx, const float dy){

    }
}

