#include <cube/data/Generator.hpp>

#include "cube/core/Block.hpp"

namespace cube {

    Generator::Generator(const int seed) : m_seed(seed) {
        m_noise = FastNoise::New<FastNoise::FractalFBm>();
        m_noise->SetSource(FastNoise::New<FastNoise::Simplex>());
        m_noise->SetOctaveCount(4);
    }

    void Generator::fill_chunk(const glm::ivec3 chunk_pos, Chunk& chunk) {
        const auto view = chunk.view();

        const int startX = chunk_pos.x * CHUNK_SIZE.x;
        const int startY = chunk_pos.y * CHUNK_SIZE.y;
        const int startZ = chunk_pos.z * CHUNK_SIZE.z;

        if (startY < 0 || startY > 256) { std::ranges::fill(chunk.data, 0); return; }

        for (int z = 0; z < CHUNK_SIZE.z; ++z) {
            for (int x = 0; x < CHUNK_SIZE.x; ++x) {
                const float noiseVal = m_noise->GenSingle2D(static_cast<float>(startX + x), static_cast<float>(startZ + z), m_seed);
                const int height = static_cast<int>(64.0f + noiseVal * 32.0f);

                for (int y = 0; y < CHUNK_SIZE.y; ++y) {
                    const int worldY = startY + y;
                    uint8_t block = 0;

                    if (worldY == height) {
                        block = GrassBlock;
                    } else if (worldY < height) {
                        block = Dirt;
                    } else if (worldY <= 64) {
                        block = Water;
                    }

                    view[x, y, z] = block;
                }
            }
        }
    }

}
