#include <format>
#include <cube/Cube.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>

#include "cube/graphics/Vertex.hpp"
#include "glad/glad.h"

namespace cube {

    constexpr auto CAMERA_SPEED = 5.0f;

    Cube::Cube() : m_window("Cube"){

    }

    void Cube::run() {

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        m_window.setRelativeMouseMode(true);
        onResize(m_window.getSize().x, m_window.getSize().y);
        uint64_t lastTime = SDL_GetTicks();

        m_camera.setPosition(glm::vec3(8.0f, 40.0f, 8.0f));

        while (m_window.isOpen()) {
            const uint64_t currentTime = SDL_GetTicks();
            const auto deltaTime = static_cast<float>(currentTime - lastTime) / 1000.0f;
            lastTime = currentTime;
            onUpdate(deltaTime);

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                onEvent(event);
            }

            glClearColor(0.7f, 0.7f, 0.8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            onDraw();
            m_window.swapBuffers();
        }
    }

    void Cube::onDraw() {

    }

    void Cube::onEvent(const SDL_Event &event) {
        if(event.type == SDL_EVENT_QUIT) {
            m_window.close();
        }
        if (event.type == SDL_EVENT_MOUSE_MOTION) {
            m_camera.rotate(event.motion.xrel, -event.motion.yrel);
        }
        if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
            onResize(event.window.data1, event.window.data2);
        }
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

        m_window.setTitle(std::format("Cube | FPS: {:3.2f} | {},{},{}", dt > 0 ? 1.0f / dt : 144.f, newPos.x, newPos.y, newPos.z));
    }

    void Cube::onResize(const int w, const int h) {
        if (h == 0) return;
        glViewport(0, 0, w, h);
        const float aspect = static_cast<float>(w) / static_cast<float>(h);
        m_projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 512.0f);
    }

}
