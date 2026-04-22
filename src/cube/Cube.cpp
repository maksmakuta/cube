#include <cube/Cube.hpp>

#include "cube/utils/Logger.hpp"

namespace cube {

    Cube::Cube() {

    }

    Cube::~Cube() {

    }

    void Cube::onDraw() {
        const auto pos = m_camera.getPosition();
        const auto dir = m_camera.getDirection();
        Log::info("Camera in [{:3.2f},{:3.2f},{:3.2f}] looking at [{:3.2f},{:3.2f}]",pos.x, pos.y, pos.z, dir.x, dir.y);
    }

    void Cube::onUpdate(float dt) {
        auto newPos = m_camera.getPosition();
        const auto buttons = SDL_GetKeyboardState(nullptr);
        if (buttons[SDL_SCANCODE_W]) newPos += m_camera.getFront() * dt * CAMERA_SPEED;
        if (buttons[SDL_SCANCODE_S]) newPos -= m_camera.getFront() * dt * CAMERA_SPEED;
        if (buttons[SDL_SCANCODE_A]) newPos -= m_camera.getRight() * dt * CAMERA_SPEED;
        if (buttons[SDL_SCANCODE_D]) newPos += m_camera.getRight() * dt * CAMERA_SPEED;
        if (buttons[SDL_SCANCODE_SPACE]) newPos += WORLD_UP * dt * CAMERA_SPEED;
        if (buttons[SDL_SCANCODE_LSHIFT]) newPos -= WORLD_UP * dt * CAMERA_SPEED;
        m_camera.move(newPos);
    }

    void Cube::onEvent(const SDL_Event& e) {
        if (e.type == SDL_EVENT_MOUSE_MOTION) {
            m_camera.rotate(glm::vec2{e.motion.xrel, -e.motion.yrel} * MOUSE_SENSITIVITY);
        }
    }

}
