#include "cube/Cube.hpp"

namespace cube {

    Cube::Cube() = default;

    void Cube::onUpdate(const float dt) {
        glm::vec3 inputDir{0.0f, 0.0f, 0.0f};

        const auto key_states = SDL_GetKeyboardState(nullptr);
        if (key_states[SDL_SCANCODE_W]) inputDir.z += 1.0f;
        if (key_states[SDL_SCANCODE_S]) inputDir.z -= 1.0f;
        if (key_states[SDL_SCANCODE_A]) inputDir.x -= 1.0f;
        if (key_states[SDL_SCANCODE_D]) inputDir.x += 1.0f;
        if (key_states[SDL_SCANCODE_SPACE]) inputDir.y += 1.0f;
        if (key_states[SDL_SCANCODE_LSHIFT]) inputDir.y -= 1.0f;

        if (glm::length(inputDir) > 0.0f) {
            inputDir = glm::normalize(inputDir);
        }

        const float speed = 5.0f * dt;
        inputDir *= speed;

        m_camera.moveFPS(inputDir.z, inputDir.x, inputDir.y);
    }

    void Cube::onDraw() {

    }

    void Cube::onEvent(const SDL_Event& event) {
        if (event.type == SDL_EVENT_MOUSE_MOTION) {
            m_camera.rotate(glm::vec2{event.motion.xrel, -event.motion.yrel} * 0.1f);
        }
    }

    ChunkState Cube::getChunkState(const glm::ivec3& pos) {
        if (!m_states.contains(pos)) {
            m_states[pos] = ChunkState::Requested;
        }
        return m_states[pos];
    }

}
