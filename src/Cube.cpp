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
        Renderer::clear(0xFF00FF00);

        m_renderer.use();
        m_renderer.fill(0xFFFF0000);
        m_renderer.rect(0,0,300,300);
    }

    void Cube::onUpdate(float dt) {

    }

    void Cube::onResize(int w, int h){
        printf("resize to %ix%i\n",w,h);
        m_renderer.onResize(w,h);
    }

    void Cube::onKey(int k, int a, int m){

    }

    void Cube::onCursor(float x, float y){

    }

    void Cube::onScroll(float dx, float dy){

    }
}

