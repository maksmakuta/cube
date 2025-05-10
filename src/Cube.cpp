#include "cube/Cube.hpp"

#include "glad/gl.h"

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

    void Cube::onUpdate(float dt) {

    }

    void Cube::onResize(const int w, const int h){
        glViewport(0,0,w,h);
        m_renderer.onResize(w,h);
        m_voxel.onResize(w,h);
    }

    void Cube::onKey(int k, int a, int m){

    }

    void Cube::onCursor(float x, float y){

    }

    void Cube::onScroll(float dx, float dy){

    }
}

