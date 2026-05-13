#include "cube/Cube.hpp"

#include <random>
#include <glm/gtx/norm.hpp>

#include "cube/mesh/Mesher.hpp"
#include "cube/utils/Log.hpp"

namespace cube {

    Cube::Cube() : m_frame_count(0), m_last_tick(0.f), m_time(0.f), m_view(0), m_last_chunk(-99999999){
        for (unsigned int i = 0; i < std::thread::hardware_concurrency() - 1; ++i) {
            m_workers.emplace_back([this] { workerLoop(); });
        }

        std::random_device rd;
        std::default_random_engine generator(rd());
        std::uniform_int_distribution distribution;
        m_generator.setSeed(distribution(generator));

        auto pos = m_camera.getPosition();
        pos.y = static_cast<float>(m_generator.heightAt({0,0,0}) + 2);
        m_camera.setPosition(pos);
    }

    void Cube::onUpdate(const float dt) {
        m_time += dt;
        m_last_tick += dt;
        m_frame_count++;

        if (m_last_tick >= 1.0f) {
            debug("FPS: {} | FrameTime: {:.3f}ms",
                m_frame_count,
                m_last_tick / static_cast<float>(m_frame_count) * 1000.0f
            );

            m_last_tick = 0.0f;
            m_frame_count = 0;
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

            std::vector<glm::ivec3> chunk_positions;
            chunk_positions.reserve(static_cast<size_t>(std::pow(RENDER_DIST * 2, 3)));

            for (int z = -RENDER_DIST; z < RENDER_DIST; z++) {
                for (int x = -RENDER_DIST; x < RENDER_DIST; x++) {
                    for (int y = -RENDER_DIST; y < RENDER_DIST; y++) {
                        glm::ivec3 offset{x, y, z};

                        if (glm::length(glm::vec3(offset)) > RENDER_DIST) continue;

                        const auto new_chunk_pos = current_chunk + offset;
                        if (!m_world.contains(new_chunk_pos)) {
                            chunk_positions.push_back(new_chunk_pos);
                        }
                    }
                }
            }

            std::sort(chunk_positions.begin(), chunk_positions.end(),
            [&current_chunk](const glm::ivec3& a, const glm::ivec3& b) {
                return glm::distance2(glm::vec3(current_chunk), glm::vec3(a)) <
                       glm::distance2(glm::vec3(current_chunk), glm::vec3(b));
            });

            // 3. Push the sorted chunks into your generation queue
            for (const auto& pos : chunk_positions) {
                m_gq.push(pos);
            }

            m_last_chunk = current_chunk;

            m_world.clearChunks(m_last_chunk,RENDER_DIST);
            m_renderer.clearChunks(m_last_chunk,RENDER_DIST);
        }

    }

    void Cube::onDraw() {
        m_renderer.draw(
            m_camera.getViewMatrix(),
            m_camera.getProjMatrix(m_view, (RENDER_DIST + 4) * CHUNK_SIZE),
            m_time
        );
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

    void Cube::workerLoop() {
        while (m_running) {
            bool worked = false;

            if (auto pos = m_gq.pop()) {
                if (!m_world.contains(*pos)) {
                    auto chunk = m_generator.generateChunk(*pos);
                    m_world.setChunk(*pos, std::move(chunk));
                    m_mq.push(*pos);
                }
                worked = true;
            }

            if (auto pos = m_mq.pop()) {
                if (isReadyForMesh(*pos,m_world)) {
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
