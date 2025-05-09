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
        clear(0xFF202020);
        m_renderer.stroke(0xFF00FF00,5,LineCap::Butt,LineJoint::Miter);
        m_renderer.rect(mouse.x - 50,mouse.y - 50, 100,100);
    }

    void Cube::onUpdate(float dt) {

    }

    void Cube::onResize(const int w, const int h){
        m_renderer.onResize(w,h);
    }

    void Cube::onKey(int k, int a, int m){

    }

    void Cube::onCursor(float x, float y){
        mouse = {x,y};
    }

    void Cube::onScroll(float dx, float dy){

    }
}

