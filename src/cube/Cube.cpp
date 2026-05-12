#include "cube/Cube.hpp"

#include "cube/mesh/Mesher.hpp"
#include "cube/utils/Log.hpp"

namespace cube {

    constexpr auto RENDER_DIST = 16;

    Cube::Cube(const int argc, char** argv): m_last_tick(0.f), m_view(0), m_last_chunk(-99999999) {
        for (auto i = 0; i < argc; i++) {
            info("args[{}]: {}", i, argv[i]);
        }

        for (unsigned int i = 0; i < std::thread::hardware_concurrency() - 1; ++i) {
            m_workers.emplace_back([this] { workerLoop(); });
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

        int uploads = 0;
        while (auto result = m_rq.pop()) {
            m_renderer.push(result->first, result->second);
            if (++uploads > 16) break;
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

            m_world.clearChunks(m_last_chunk,RENDER_DIST);
            m_renderer.clearChunks(m_last_chunk,RENDER_DIST);
        }

    }

    void Cube::onDraw() {
        m_renderer.draw(m_camera.getViewMatrix(), m_camera.getProjMatrix(m_view, (RENDER_DIST + 4) * CHUNK_SIZE));
    }

    void Cube::onEvent(const SDL_Event& event) {
        if (event.type == SDL_EVENT_MOUSE_MOTION) {
            m_camera.applyDirection(glm::vec2{event.motion.xrel, -event.motion.yrel} * 0.1f);
        }
        if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
            m_view = glm::vec2{event.window.data1, event.window.data2};
        }
        if (event.type == SDL_EVENT_QUIT) {
            m_running = false;
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

    void Cube::workerLoop() {
        while (m_running) {
            bool worked = false;

            // 1. Try Generation
            if (auto pos = m_gq.pop()) {
                if (!m_world.contains(*pos)) {
                    auto chunk = m_generator.generateChunk(*pos);
                    {
                        m_world.setChunk(*pos, std::move(chunk));
                    }
                    m_mq.push(*pos);
                }
                worked = true;
            }

            // 2. Try Meshing
            if (auto pos = m_mq.pop()) {
                bool ready = false;
                {

                    ready = isReadyForMesh(*pos);
                }

                if (ready) {
                    const auto mesh_data = mesh(*pos, m_world);
                    m_rq.push({*pos, mesh_data});
                } else {
                    m_mq.push(*pos);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
                worked = true;
            }

            if (!worked) {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        }
    }

}
