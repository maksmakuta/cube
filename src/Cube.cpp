#include "cube/Cube.hpp"

namespace cube {

    Cube::Cube() = default;

    void Cube::onDraw() {
        m_renderer.draw();
    }

    void Cube::onResize(int w, int h){
        m_renderer.resize(w,h);
    }

    void Cube::onKey(int k, int a, int m){

    }

    void Cube::onCursor(float x, float y){

    }

    void Cube::onScroll(float dx, float dy){

    }
}

