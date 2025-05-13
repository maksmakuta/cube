#include "cube/Cube.hpp"

#include <iostream>
#include <GLFW/glfw3.h>

#include "cube/core/Constants.hpp"
#include "cube/graphics/renderer/Renderer.hpp"

namespace cube {

    Cube::Cube() = default;

    void Cube::onCreate() {
    }

    void Cube::onClear() {
    }

    void Cube::onDraw() {
        clear(0xFF202020);
    }

    void Cube::onTick() {
        std::cout << "TICK" << std::endl;
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
    }

    void Cube::onScroll(const float dx, const float dy){

    }
}

