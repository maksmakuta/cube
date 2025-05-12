#include "cube/Cube.hpp"
#include "glad/gl.h"

#include <GLFW/glfw3.h>

#include "cube/graphics/renderer/Renderer.hpp"

namespace cube {

    Cube::Cube() = default;

    void Cube::onCreate() {

        // const auto x = 0;
        // const auto y = 0;

        const auto r = static_cast<int>(RENDER_DIST);
        for (int x = -r; x <= r; ++x) {
        for (int y = -r; y <= r; ++y) {
                auto c = GeneratorFlat().generateAt({x,y});
                m_world.meshes[{x,y}] = Mesh::toMesh(*c);
        }
        }

        m_voxel.onCreate();

    }

    void Cube::onClear() {
        m_voxel.onClear();
    }

    void Cube::onDraw() {
        clear(0xFF202020);
        for (const auto& [offset, mesh] : m_world.meshes) {
            m_voxel.draw(mesh,offset,m_player.getCamera().getView());
        }
    }

    void Cube::onUpdate(const float dt) {
        m_player.move(m_direction,dt);
        m_voxel.setLigth(m_player.getPosition() + glm::vec3(0,CHUNK_HEIGHT,0));
        // last_tick += dt;
        // if (last_tick > TICK) {
        //     last_tick = 0;
        // }
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

