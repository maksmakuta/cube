#include "cube/Cube.hpp"

#include "cube/mesh/Mesher.hpp"
#include "cube/utils/Log.hpp"

namespace cube {

    constexpr auto RENDER_DIST = 8;

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

            for (int z = -RENDER_DIST; z < RENDER_DIST; z++) {
                for (int x = -RENDER_DIST; x < RENDER_DIST; x++) {
                    for (int y = -RENDER_DIST; y < RENDER_DIST; y++) {
                        const auto new_chunk_pos = current_chunk + glm::ivec3{x, y, z};
                        if (!m_world.contains(new_chunk_pos)) {
                            m_gq.push(current_chunk + glm::ivec3{x, y, z});
                        }
                    }
                }
            }

            m_last_chunk = current_chunk;
        }

        int limit = 16;
        int count = 0;

        while(!m_gq.empty()) {
            const auto pos = m_gq.front();
            m_gq.pop();

            if (!m_world.contains(pos)) {
                m_world.setChunk(pos, m_generator.generateChunk(pos));
                m_mq.push(pos);
            }
            if (count >= limit) {
                break;
            }
            ++count;
        }

        count = 0;

        while(!m_mq.empty()) {
            const auto pos = m_mq.front();
            m_mq.pop();

            if (pos.y < 0 || pos.y > 16 || !m_renderer.contains(pos)) {
                if (isReadyForMesh(pos)) {
                    const auto mesh_data = mesh(pos,m_world);
                    m_renderer.push(pos,mesh_data);
                } else {
                    m_mq.push(pos);
                }
            }

            if (count >= limit) {
                break;
            }
            ++count;
        }

        debug("G: {} | M: {} | W: {} | R: {} | {}",
            m_gq.size(),
            m_mq.size(),
            m_world.clearChunks(m_last_chunk,RENDER_DIST),
            m_renderer.clearChunks(m_last_chunk,RENDER_DIST),
            1.0 / dt
        );

    }

    void Cube::onDraw() {
        m_renderer.draw(m_camera.getViewMatrix(), m_camera.getProjMatrix(m_view, RENDER_DIST * CHUNK_SIZE));
    }

    void Cube::onEvent(const SDL_Event& event) {
        if (event.type == SDL_EVENT_MOUSE_MOTION) {
            m_camera.applyDirection(glm::vec2{event.motion.xrel, -event.motion.yrel} * 0.1f);
        }
        if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
            m_view = glm::vec2{event.window.data1, event.window.data2};
        }
    }

    const glm::ivec3 directions[6] = {
        {1,0,0},
        {-1,0,0},
        {0,1,0},
        {0,-1,0},
        {0,0,1},
        {0,0,-1}
    };

    bool Cube::isReadyForMesh(const glm::ivec3 &pos) const {
        if (m_world.contains(pos)) {
            return std::ranges::all_of(directions, [&](const glm::ivec3& direction) {
                return m_world.contains(pos + direction);
            });
        }
        return false;
    }

}
