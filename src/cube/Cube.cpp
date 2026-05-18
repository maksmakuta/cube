#include "cube/Cube.hpp"

namespace cube {

    inline glm::ivec3 worldToChunkPos(const glm::vec3& worldPos) {
        return glm::floor(worldPos / static_cast<float>(CHUNK_SIZE));
    }

    Cube::Cube() : m_gen_pool(4), m_mesh_pool(4) { }

    void Cube::onUpdate(const float dt) {
        auto speed = 25.f * dt;
        auto delta_pos = glm::vec3();
        const auto keys = SDL_GetKeyboardState(nullptr);
        if (keys[SDL_SCANCODE_W])       { delta_pos += m_camera.getFront(); }
        if (keys[SDL_SCANCODE_S])       { delta_pos -= m_camera.getFront(); }
        if (keys[SDL_SCANCODE_A])       { delta_pos -= m_camera.getRight(); }
        if (keys[SDL_SCANCODE_D])       { delta_pos += m_camera.getRight(); }
        if (keys[SDL_SCANCODE_LSHIFT])  { delta_pos -= glm::vec3(0.f,1.f,0.f); }
        if (keys[SDL_SCANCODE_SPACE])   { delta_pos += glm::vec3(0.f,1.f,0.f); }
        if (keys[SDL_SCANCODE_LCTRL])   { speed *= 4.f; }

        const auto new_pos = m_camera.getPosition() + delta_pos * speed;

        m_camera.moveTo(new_pos);
        
        const auto current_player_chunk = worldToChunkPos(new_pos);

        m_renderer.unloadChunks(current_player_chunk, RENDER_DIST);
        m_world.unloadChunks(current_player_chunk, RENDER_DIST);

        int generation_dispatched = 0;
        int meshing_dispatched = 0;
        constexpr int max_dispatched_per_frame = 3;

        for (int x = -RENDER_DIST; x <= RENDER_DIST; ++x) {
            for (int z = -RENDER_DIST; z <= RENDER_DIST; ++z) {
                for (int y = -RENDER_DIST; y <= RENDER_DIST; ++y) {
                    const auto target_pos = current_player_chunk + glm::ivec3(x, y, z);

                    if (glm::distance(glm::vec3(current_player_chunk), glm::vec3(target_pos)) > RENDER_DIST) {
                        continue;
                    }

                    if (!m_world.getChunk(target_pos) && generation_dispatched < max_dispatched_per_frame) {
                        m_world.setStatus(target_pos, Status::Empty);
                        generation_dispatched++;

                        m_gen_pool.enqueue([this, target_pos] {
                            auto chunk = m_generator.generate(target_pos);
                            m_world.setChunk(target_pos, std::move(chunk));
                            m_world.setStatus(target_pos, Status::Present);
                        });
                        continue;
                    }

                    if (m_world.getStatus(target_pos) == Status::Present && meshing_dispatched < max_dispatched_per_frame) {
                        if (m_world.readyToMesh(target_pos)) {
                            m_world.setStatus(target_pos, Status::Meshed);
                            meshing_dispatched++;

                            m_mesh_pool.enqueue([this, target_pos] {
                                auto mesh = generateMesh(target_pos, m_world);
                                m_meshQueue.push(MeshResult{target_pos, std::move(mesh)});
                            });
                        }
                    }
                }
            }
        }

        int uploads_this_frame = 0;
        while (uploads_this_frame < 4) {
            auto result = m_meshQueue.pop();
            if (!result.has_value()) {
                break;
            }

            m_renderer.uploadChunk(result->pos, result->mesh);
            m_world.setStatus(result->pos, Status::Loaded);
            uploads_this_frame++;
        }
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