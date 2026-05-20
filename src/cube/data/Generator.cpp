#include "cube/data/Generator.hpp"

#include "cube/core/BlockID.hpp"

namespace cube {

    Generator::Generator(const int seed) : m_seed(seed) {

        auto base_noise = FastNoise::New<FastNoise::Simplex>();
        base_noise->SetScale(2048.f);
        base_noise->SetSeedOffset(-1);

        auto terrain_noise = FastNoise::New<FastNoise::FractalRidged>();
        terrain_noise->SetSource(base_noise);
        terrain_noise->SetOctaveCount(4);
        terrain_noise->SetGain(0.34);
        terrain_noise->SetLacunarity(1.73);

        m_terrain = terrain_noise;

    }

    std::shared_ptr<Chunk> Generator::generate(const glm::ivec3& pos) {
        auto chunk = std::make_shared<Chunk>();

        if (pos.y < 0 || pos.y > 16) {
            return chunk;
        }

        std::array<float, CHUNK_SIZE * CHUNK_SIZE> terrain_noise{};

        m_terrain->GenUniformGrid2D(
            terrain_noise.data(),
            static_cast<float>(pos.x * CHUNK_SIZE),
            static_cast<float>(pos.z * CHUNK_SIZE),
            CHUNK_SIZE,
            CHUNK_SIZE,
            1.f,
            1.f,
            m_seed
        );

        constexpr int WATER_LEVEL = 64;

        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                const auto index = z * CHUNK_SIZE + x;

                const auto value = terrain_noise[index];
                const auto terrain = static_cast<int>(value * 64.f) + 96;

                for (int y = 0; y < CHUNK_SIZE; y++) {
                    const auto worldY = pos.y * CHUNK_SIZE + y;

                    auto voxel = Air;

                    if (worldY == 0) {
                        voxel = Bedrock;
                    }
                    else if (worldY < 32) {
                        voxel = Deepslate;
                    }
                    else if (worldY < terrain - 4) {
                        voxel = Stone;
                    }
                    else if (worldY < terrain) {
                        voxel = Dirt;
                    }
                    else if (worldY == terrain) {
                        voxel = GrassBlock;
                    }
                    else if (worldY <= WATER_LEVEL) {
                        voxel = Water;
                    }
                    else {
                        continue;
                    }

                    chunk->at({x, y, z}) = voxel;
                }
            }
        }

        return chunk;
    }

    int Generator::getHeightAt(const glm::ivec3&) {
        return 32;
    }

}
