#include "cube/Cube.hpp"
#include "glad/gl.h"

#include <GLFW/glfw3.h>

namespace cube {

    Cube::Cube() = default;

    void Cube::onCreate() {
        m_renderer.onCreate();
        m_voxel.onCreate();
    }

    void Cube::onClear() {
        m_renderer.onClear();
        m_voxel.onClear();
    }

    void Cube::onDraw() {
        clear(0xFF202020);
    }

    void Cube::onUpdate(const float dt) {
        m_player.move(m_direction,dt);
    }

    void Cube::onResize(const int w, const int h){
        glViewport(0,0,w,h);
        m_renderer.onResize(w,h);
        m_voxel.onResize(w,h);
    }

    void Cube::onKey(const int k, const int a, const int m){
        if (k == GLFW_KEY_F1 && a == GLFW_PRESS) {
            m_move_player = !m_move_player;
        }

    }

    void Cube::onCursor(const float x, const float y){
        m_player.rotate(x,-y);
    }

    void Cube::onScroll(const float dx, const float dy){

    }
}

