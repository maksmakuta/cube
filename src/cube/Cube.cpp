#include "cube/Cube.hpp"

#include "cube/utils/Log.hpp"

namespace cube {

    ChunkPtr getFlatChunk() {
        auto chunk = std::make_shared<Chunk>();

        for(int y = 0; y < CHUNK_SIZE; y++) {

            auto id = Block::Air;

            if (y == 0)
                id = Block::Bedrock;
            if (y >= 1 && y < 6)
                id = Block::Stone;
            if (y >= 6 && y < 8)
                id = Block::Dirt;
            if (y == 8)
                id = Block::Grass;

            for(int x = 0; x < CHUNK_SIZE; x++) {
                for(int z = 0; z < CHUNK_SIZE; z++) {
                    chunk->at({x,y,z}) = id;
                }
            }
        }

        return chunk;
    }

    Cube::Cube(const int argc, char** argv): m_last_tick(0.f) {
        for (auto i = 0; i < argc; i++) {
            info("args[{}]: {}", i, argv[i]);
        }

        const auto chunk_pos = glm::ivec3{0};

        m_world.setChunk(chunk_pos,getFlatChunk());
    }

    void Cube::onUpdate(const float dt) {
        m_last_tick += dt;
        if (m_last_tick >= 1.f) {
            debug("FPS: {}", m_last_tick / dt);
            m_last_tick = 0.f;
        }
    }

    void Cube::onDraw() {

    }

    void Cube::onEvent(const SDL_Event& event) {

    }

}
