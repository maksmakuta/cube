#include <cube/Cube.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <SDL3/SDL_events.h>

#include "glad/glad.h"

namespace cube {

    constexpr auto CAMERA_SPEED = 5.0f;

    Cube::Cube() : m_generator(0), m_worker(m_world){

    }

    void Cube::onDraw() {

    }

    void Cube::onEvent(const SDL_Event &event) {

    }

    void Cube::onUpdate(const float dt) {
        const auto keys = SDL_GetKeyboardState(nullptr);
        auto newPos = m_camera.getPosition();
        if (keys[SDL_SCANCODE_W]        ) { newPos += m_camera.getForward() * dt * CAMERA_SPEED; }
        if (keys[SDL_SCANCODE_S]        ) { newPos -= m_camera.getForward() * dt * CAMERA_SPEED; }
        if (keys[SDL_SCANCODE_A]        ) { newPos -= m_camera.getRight() * dt * CAMERA_SPEED; }
        if (keys[SDL_SCANCODE_D]        ) { newPos += m_camera.getRight() * dt * CAMERA_SPEED; }
        if (keys[SDL_SCANCODE_SPACE]    ) { newPos += m_camera.getUp() * dt * CAMERA_SPEED; }
        if (keys[SDL_SCANCODE_LSHIFT]   ) { newPos -= m_camera.getUp() * dt * CAMERA_SPEED; }

        m_camera.setPosition(newPos);
    }

    void Cube::onResize(const int w, const int h) {
        if (h == 0) return;
        glViewport(0, 0, w, h);
        const float aspect = static_cast<float>(w) / static_cast<float>(h);
        m_projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 512.0f);
    }

}
