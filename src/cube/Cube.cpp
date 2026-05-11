#include "cube/Cube.hpp"

#include "cube/mesh/Mesher.hpp"
#include "cube/utils/Log.hpp"

namespace cube {

    Cube::Cube(const int argc, char** argv): m_last_tick(0.f), m_view(0), m_last_chunk(-99999999) {
        for (auto i = 0; i < argc; i++) {
            info("args[{}]: {}", i, argv[i]);
        }
    }

    void Cube::onUpdate(const float dt) {
        m_last_tick += dt;
        if (m_last_tick >= 1.f) {
            debug("FPS: {}", m_last_tick / dt);
            m_last_tick = 0.f;
        }

        auto delta_move = glm::vec3();
        const auto keys = SDL_GetKeyboardState(nullptr);
        if (keys[SDL_SCANCODE_W]) delta_move += m_camera.getForward();
        if (keys[SDL_SCANCODE_S]) delta_move -= m_camera.getForward();
        if (keys[SDL_SCANCODE_A]) delta_move -= m_camera.getRight();
        if (keys[SDL_SCANCODE_D]) delta_move += m_camera.getRight();
        if (keys[SDL_SCANCODE_SPACE]) delta_move += glm::vec3{0,1,0};
        if (keys[SDL_SCANCODE_LSHIFT]) delta_move -= glm::vec3{0,1,0};

        if (glm::length(delta_move) > 0.f) {
            m_camera.setPosition(m_camera.getPosition() + glm::normalize(delta_move) * dt * 25.f);
        }

        const auto current_chunk = glm::ivec3(glm::floor(m_camera.getPosition() / static_cast<float>(CHUNK_SIZE)));
        if (current_chunk != m_last_chunk) {
            constexpr auto RENDER_DIST = 4;

            for (int z = -RENDER_DIST; z < RENDER_DIST; z++) {
                for (int x = -RENDER_DIST; x < RENDER_DIST; x++) {
                    for (int y = -RENDER_DIST; y < RENDER_DIST; y++) {
                        const auto new_chunk_pos = current_chunk + glm::ivec3{x, y, z};
                        if (!m_world.contains(new_chunk_pos)) {
                            m_generator.push(current_chunk + glm::ivec3{x, y, z});
                        }
                    }
                }
            }

            m_last_chunk = current_chunk;
        }

        if (m_generator.len() > 0) {
            glm::ivec3 pos;
            const auto chunk = m_generator.pop(&pos);
            m_world.setChunk(pos, chunk);

            m_mesher.push(pos);
        }

        if (!m_mesher.empty()) {
            const auto pos = m_mesher.front();
            m_mesher.pop();

            const auto mesh_data = mesh(pos,m_world);
            m_renderer.push(pos,mesh_data);
        }

        debug("in generation: {}", m_generator.len());
        debug("in meshing: {}", m_mesher.size());
    }

    void Cube::onDraw() {
        m_renderer.draw(m_camera.getViewMatrix(), m_camera.getProjMatrix(m_view,128));
    }

    void Cube::onEvent(const SDL_Event& event) {
        if (event.type == SDL_EVENT_MOUSE_MOTION) {
            m_camera.applyDirection(glm::vec2{event.motion.xrel, -event.motion.yrel} * 0.1f);
        }
        if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
            m_view = glm::vec2{event.window.data1, event.window.data2};
        }
    }

}
