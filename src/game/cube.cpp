#include "cube/game/cube.hpp"
#include "cube/graphics/gl/glad.h"

namespace cube::game {

    void Cube::init(){
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

    }

    void Cube::clear(){

    }

    void Cube::draw(){
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Cube::update(float delta){

    }

    void Cube::onResize(int w, int h){

    }

    void Cube::onCursor(float x, float y){

    }

    void Cube::onKey(int key, int action, int mods){

    }

}
