#include "cube/Cube.hpp"

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

    }

    void Cube::onUpdate(float dt) {

    }

    void Cube::onResize(const int w, const int h){
        m_renderer.onResize(w,h);
    }

    void Cube::onKey(int k, int a, int m){

    }

    void Cube::onCursor(float x, float y){

    }

    void Cube::onScroll(float dx, float dy){

    }
}

