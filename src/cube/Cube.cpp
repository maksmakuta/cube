#include "cube/Cube.hpp"

#include "cube/mesh/Mesher.hpp"
#include "cube/utils/Log.hpp"

namespace cube {

    ChunkPtr getFlatChunk() {
        auto chunk = std::make_shared<Chunk>();

        for (int y = 0; y < CHUNK_SIZE; y++) {
            auto id = Block::Air;

            if (y == 0) {
                id = Block::Bedrock;
            } else if (y < 6) {
                id = Block::Stone;
            } else if (y < 8) {
                id = Block::Dirt;
            } else if (y == 8) {
                id = Block::Grass;
            } else {
                continue;
            }

            for (int x = 0; x < CHUNK_SIZE; x++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    chunk->at({x, y, z}) = id;
                }
            }
        }

        return chunk;
    }

    Cube::Cube(const int argc, char** argv): m_last_tick(0.f), m_view(0) {
        for (auto i = 0; i < argc; i++) {
            info("args[{}]: {}", i, argv[i]);
        }

        const auto chunk_pos = glm::ivec3{0};

        m_world.setChunk(chunk_pos,getFlatChunk());
        m_renderer.push(chunk_pos,mesh(chunk_pos,m_world));

        if (m_world.contains(chunk_pos)) {
            warn("Chunk is present in world");
            if (m_renderer.contains(chunk_pos)) {
                warn("Chunk is present in renderer");
            }
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
