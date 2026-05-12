#include "cube/data/Generator.hpp"

namespace cube {

    Generator::Generator(const int seed) : m_seed(seed) {
        m_terrain = FastNoise::New<FastNoise::Perlin>();
        m_terrain->SetScale(64.f);
    }

    ChunkPtr Generator::generateChunk(const glm::ivec3 &pos) {
        auto chunk = std::make_shared<Chunk>();

        if (pos.y < 0 || pos.y >= 16) {
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

    int Generator::heightAt(const glm::vec3 &pos) const {
        const auto value = m_terrain->GenSingle2D(pos.x, pos.z,m_seed);
        return static_cast<int>(value * 32.f) + 64;
    }

    int Generator::getSeed() const {
        return m_seed;
    }

    void Generator::setSeed(const int seed) {
        m_seed = seed;
    }

}
