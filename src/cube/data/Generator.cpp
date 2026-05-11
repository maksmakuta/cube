#include "cube/data/Generator.hpp"

namespace cube {

    Generator::Generator(const int seed) : m_seed(seed) {
        m_terrain = FastNoise::New<FastNoise::Perlin>();
        m_terrain->SetScale(256.f);
    }

    void Generator::push(const glm::ivec3& chunk) {
        m_chunk.push(chunk);
    }

    void Generator::clear() {
        while (!m_chunk.empty()) {
            m_chunk.pop();
        }
    }

    ChunkPtr Generator::pop(glm::ivec3* pos) {
        if (m_chunk.empty()) {
            return nullptr;
        }

        const auto p = m_chunk.front();
        m_chunk.pop();

        if (pos != nullptr) {
            *pos = p;
        }

        return generateChunk(p);
    }

    int Generator::len() const {
        return static_cast<int>(m_chunk.size());
    }

    ChunkPtr Generator::generateChunk(const glm::ivec3 &pos) {
        auto chunk = std::make_shared<Chunk>();

        if (pos.y < 0 || pos.y * CHUNK_SIZE > WORLD_LIMIT) {
            return chunk;
        }

        std::vector<float> terrain_data(CHUNK_SIZE * CHUNK_SIZE);

        m_terrain->GenUniformGrid2D(
            terrain_data.data(),
            static_cast<float>(pos.x * CHUNK_SIZE),
            static_cast<float>(pos.z * CHUNK_SIZE),
            CHUNK_SIZE,
            CHUNK_SIZE,
            1.f,
            1.f,
            m_seed
        );

        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {

                const auto value = terrain_data[z * CHUNK_SIZE + x];
                const auto height = static_cast<int>(value * 32.f) + 64;

                for (int cy = 0; cy < CHUNK_SIZE; cy++) {
                    const int y = pos.y * CHUNK_SIZE + cy;
                    auto id = Block::Air;

                    if (y == 0) {
                        id = Block::Bedrock;
                    } else if (y < height - 4) {
                        id = Block::Stone;
                    } else if (y < height) {
                        id = Block::Dirt;
                    } else if (y == height) {
                        id = Block::Grass;
                    } else {
                        continue;
                    }

                    chunk->at({x, cy, z}) = id;

                }
            }
        }

        return chunk;
    }

}
