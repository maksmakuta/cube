#include "cube/game/cube.hpp"
#include "cube/graphics/gl/glad.h"
#include "cube/utils/utils.hpp"

#include <iostream>
#include <GLFW/glfw3.h>

namespace cube::game {

    void Cube::init(){
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    }

    void Cube::clear(){

    }

    void Cube::draw(){
        glClear(GL_COLOR_BUFFER_BIT);

        const auto pos = m_camera.getPosition();
        const auto rot = m_camera.getRotation();

        std::cout << std::format("pos : [{:3.2f},{:3.2f},{:3.2f}] rot : [{:3.2f},{:2.2f}]",pos.x,pos.y,pos.z,rot.x,rot.y) << std::endl;
    }

    void Cube::update(const float delta){
        m_camera.update(direction,delta);
    }

    void Cube::onResize(const int w, const int h){

    }

    void Cube::onCursor(const float x, const float y){
        const auto curr = glm::vec2(x,-y);
        m_camera.rotate(curr - last);
        last = curr;
    }

    void Cube::onKey(const int key, const int action, const int mods){
        if (key == GLFW_KEY_W) {
            utils::setBit(direction,render::CameraDirection::FORWARD,action != GLFW_RELEASE);
        }
        if (key == GLFW_KEY_S) {
            utils::setBit(direction,render::CameraDirection::BACKWARD,action != GLFW_RELEASE);
        }
        if (key == GLFW_KEY_A) {
            utils::setBit(direction,render::CameraDirection::LEFT,action != GLFW_RELEASE);
        }
        if (key == GLFW_KEY_D) {
            utils::setBit(direction,render::CameraDirection::RIGHT,action != GLFW_RELEASE);
        }
        if (key == GLFW_KEY_SPACE) {
            utils::setBit(direction,render::CameraDirection::UP,action != GLFW_RELEASE);
        }
        if (key == GLFW_KEY_LEFT_SHIFT) {
            utils::setBit(direction,render::CameraDirection::DOWN,action != GLFW_RELEASE);
        }
    }

}
