#include <random>
#include <ranges>
#include <cube/Cube.hpp>
#include <cube/utils/Logger.hpp>

#include <FastNoise/FastNoise.h>
#include <glad/glad.h>

namespace cube {

    constexpr auto DIST = 4;

    int randomSeed() {
        std::random_device dev;
        std::default_random_engine r(dev());
        const auto seed =  static_cast<int>(r());
        Log::info("Seed : {}", seed);
        return seed;
    }

    Cube::Cube() : m_world(randomSeed()){}

    Cube::~Cube() = default;

    void Cube::onDraw() {
        m_renderer.render(m_projection, m_camera.getViewMatrix(),m_camera.getPosition());
    }

    void Cube::onUpdate(const float dt) {
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
        if (e.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
            const auto aspect = static_cast<float>(e.window.data1) / static_cast<float>(e.window.data2);
            m_projection = glm::perspective(glm::radians(45.f),aspect,0.1f,512.f);
        }
    }

}
