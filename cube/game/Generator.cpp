#include "Generator.hpp"

namespace cube {

    constexpr auto BASE_HEIGHT = 72;
    constexpr auto WATER_LEVEL = 64;

    Generator::Generator(const uint32_t seed) : m_seed(seed) {
        const auto baseNode = FastNoise::New<FastNoise::Perlin>();
        const auto noise = FastNoise::New<FastNoise::FractalFBm>();
        noise->SetSource(baseNode);
        noise->SetOctaveCount(3);
        m_noise = noise;
    }

    float Generator::getHeight(const glm::vec2& pos) const {
        return m_noise->GenSingle2D(pos.x,pos.y,static_cast<int>(m_seed));
    }

    ChunkPtr Generator::generateChunk(const glm::ivec2& pos) const {
        std::vector<float> heightmap(CHUNK_SIZE.x * CHUNK_SIZE.z,BASE_HEIGHT);
        const glm::ivec2 world_pos = pos * glm::ivec2(CHUNK_SIZE.x,CHUNK_SIZE.z);
        m_noise->GenUniformGrid2D(
            heightmap.data(),
            world_pos.x,world_pos.y,
            CHUNK_SIZE.x,CHUNK_SIZE.z,
            0.01f,static_cast<int>(m_seed)
        );
        const auto chunk = std::make_shared<Chunk>();

        for (auto x = 0; x < CHUNK_SIZE.x; x++) {
            for (auto z = 0; z < CHUNK_SIZE.z; z++) {
                const float q = heightmap[x + z * CHUNK_SIZE.x];
                int height = static_cast<int>(q * CHUNK_SIZE.y);
                for (auto y = 0; y < height; y++) {
                    chunk->setBlock({x,y,z},Block::Stone);
                }
                chunk->setBlock({x,height,z},Block::Grass);
            }
        }

        return chunk;
    }

}