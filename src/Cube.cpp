#include "cube/Cube.hpp"

#include "glad/gl.h"

namespace cube {

    Cube::Cube() {
        glClearColor(.25f,.25f,.25f,1.f);
    }

    void Cube::onDraw() {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Cube::onResize(int w, int h){

    }

    void Cube::onKey(int k, int a, int m){

    }

    void Cube::onCursor(float x, float y){

    }

    void Cube::onScroll(float dx, float dy){

    }
}

