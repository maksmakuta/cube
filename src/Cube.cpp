#include "cube/Cube.hpp"
#include "glad/gl.h"

#include <GLFW/glfw3.h>

#include "cube/generators/GeneratorFlat.hpp"
#include "cube/graphics/Mesh.hpp"
#include "cube/graphics/renderer/Renderer.hpp"

namespace cube {

    Cube::Cube() = default;

    void Cube::onCreate() {
        m_chunk = GeneratorFlat().generateAt({0,0});
        m_chunk_mesh = Mesh().toMesh(m_chunk);
        m_voxel.onCreate();
    }

    void Cube::onClear() {
        m_voxel.onClear();
    }

    void Cube::onDraw() {
        clear(0xFF202020);
        m_voxel.draw(m_chunk_mesh,{0,0},m_player.getCamera().getView());
    }

    void Cube::onUpdate(const float dt) {
        m_player.move(m_direction,dt);
    }

    void Cube::onResize(const int w, const int h){
        glViewport(0,0,w,h);
        m_voxel.onResize(w,h);
    }

    template<class T, class S>
    void set(T& val, S bit, bool op) {
        op ? val |= bit : val &= ~bit; 
    }

    void Cube::onKey(const int k, const int a, const int m){
        if (k == GLFW_KEY_W) {
            set(m_direction,Forward,a == GLFW_PRESS);
        }
        if (k == GLFW_KEY_S) {
            set(m_direction,Backward,a == GLFW_PRESS);
        }
        if (k == GLFW_KEY_A) {
            set(m_direction,Left,a == GLFW_PRESS);
        }
        if (k == GLFW_KEY_D) {
            set(m_direction,Right,a == GLFW_PRESS);
        }
        if (k == GLFW_KEY_SPACE) {
            set(m_direction,Up,a == GLFW_PRESS);
        }
        if (k == GLFW_KEY_LEFT_SHIFT) {
            set(m_direction,Down,a == GLFW_PRESS);
        }
    }

    void Cube::onCursor(const float x, const float y){
        m_player.rotate(x,-y);
    }

    void Cube::onScroll(const float dx, const float dy){

    }
}

