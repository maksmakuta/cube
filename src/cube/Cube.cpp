#include "cube/Cube.hpp"

namespace cube {

    Cube::Cube() {
        const auto pos = glm::ivec3{0};
        const auto chunk = m_generator.generate(pos);
        m_world.setChunk(pos, chunk);
        const auto mesh = generateMesh(pos,m_world);
        m_renderer.uploadChunk(pos, mesh);
    }

    void Cube::onUpdate(const float dt) {
        auto speed = 25.f * dt;
        auto delta_pos = glm::vec3();
        const auto keys = SDL_GetKeyboardState(nullptr);
        if (keys[SDL_SCANCODE_W])       { delta_pos += m_camera.getFront(); }
        if (keys[SDL_SCANCODE_S])       { delta_pos -= m_camera.getFront(); }
        if (keys[SDL_SCANCODE_A])       { delta_pos -= m_camera.getRight(); }
        if (keys[SDL_SCANCODE_D])       { delta_pos += m_camera.getRight(); }
        if (keys[SDL_SCANCODE_LSHIFT])  { delta_pos -= m_camera.getUp(); }
        if (keys[SDL_SCANCODE_SPACE])   { delta_pos += m_camera.getUp(); }
        if (keys[SDL_SCANCODE_LCTRL])   { speed *= 4.f; }

        m_camera.moveTo(m_camera.getPosition() + delta_pos * speed);
    }

    void Cube::onRender() {
        m_renderer.render(m_camera);
    }

    void Cube::onEvent(const SDL_Event& event) {
        if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
            m_camera.resize({event.window.data1,event.window.data2});
        }
        if (event.type == SDL_EVENT_MOUSE_MOTION) {
            m_camera.rotate(glm::vec2{event.motion.xrel, -event.motion.yrel} * 0.1f);
        }
    }

}