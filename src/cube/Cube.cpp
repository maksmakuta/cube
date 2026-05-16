#include "cube/Cube.hpp"

namespace cube {

    Cube::Cube() {}

    void Cube::onUpdate(const float dt) {

        // auto delta_move = glm::vec3();
        // const auto keys = SDL_GetKeyboardState(nullptr);
        // if (keys[SDL_SCANCODE_W]) delta_move += m_camera.getForward();
        // if (keys[SDL_SCANCODE_S]) delta_move -= m_camera.getForward();
        // if (keys[SDL_SCANCODE_A]) delta_move -= m_camera.getRight();
        // if (keys[SDL_SCANCODE_D]) delta_move += m_camera.getRight();
        // if (keys[SDL_SCANCODE_SPACE]) delta_move += glm::vec3{0,1,0};
        // if (keys[SDL_SCANCODE_LSHIFT]) delta_move -= glm::vec3{0,1,0};
        //
        // if (glm::length(delta_move) > 0.f) {
        //     m_camera.setPosition(m_camera.getPosition() + glm::normalize(delta_move) * dt * 25.f);
        // }
    }

    void Cube::onDraw() {

    }

    void Cube::onEvent(const SDL_Event& event) {
    //     if (event.type == SDL_EVENT_MOUSE_MOTION) {
    //         m_camera.applyDirection(glm::vec2{event.motion.xrel, -event.motion.yrel} * 0.1f);
    //     }
    //     if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
    //         m_view = glm::vec2{event.window.data1, event.window.data2};
    //     }
    //     if (event.type == SDL_EVENT_QUIT) {
    //         m_running = false;
    //     }
    }

}
