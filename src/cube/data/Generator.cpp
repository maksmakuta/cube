#include <cube/data/Generator.hpp>

#include "cube/data/Biome.hpp"

namespace cube {

    Generator::Generator(const int seed) : m_seed(seed) {
        auto terrain_noise = FastNoise::New<FastNoise::Perlin>();
        terrain_noise->SetScale(384.f);
        terrain_noise->SetSeedOffset(-1);

        auto terrain_details = FastNoise::New<FastNoise::Perlin>();
        terrain_details->SetScale(32.f);
        terrain_details->SetSeedOffset(-2);

        auto terrain_fractal = FastNoise::New<FastNoise::FractalFBm>();
        terrain_fractal->SetSource(terrain_noise);
        terrain_fractal->SetWeightedStrength(terrain_details);
        terrain_fractal->SetOctaveCount(3);
        terrain_fractal->SetLacunarity(1.75);
        m_terrain = terrain_fractal;

        auto temperature_noise = FastNoise::New<FastNoise::Perlin>();
        temperature_noise->SetScale(768.f);
        temperature_noise->SetSeedOffset(1);
        m_temperature = temperature_noise;

        auto humidity_noise = FastNoise::New<FastNoise::Perlin>();
        humidity_noise->SetScale(768.f);
        humidity_noise->SetSeedOffset(2);
        m_humidity = humidity_noise;

        m_tree = FastNoise::New<FastNoise::SuperSimplex>();
        m_decorations = FastNoise::New<FastNoise::SuperSimplex>();
    }

    Chunk Generator::generate(const glm::ivec3& chunkPos) const {
        Chunk chunk;
        auto view = chunk.view();

        const auto height_map = getTerrain(chunkPos);
        const auto temp_map   = getTemperature(chunkPos);
        const auto humid_map  = getHumidity(chunkPos);

        const int chunk_base_y = chunkPos.y * CHUNK_SIZE.y;

        for (int z = 0; z < CHUNK_SIZE.z; z++) {
            for (int x = 0; x < CHUNK_SIZE.x; x++) {
                const int idx = x + CHUNK_SIZE.x * z;

                const BiomeType type = getBiome(temp_map[idx], humid_map[idx]);
                const auto [terrain, subterrain, onwater, underwater1, underwater2] = getTerrainData(type);

                const int terrain_h = WATER_LEVEL + static_cast<int>(height_map[idx] * 48.f);
                int y = CHUNK_SIZE.y - 1;

                for (; y >= 0; --y) {
                    int world_y = chunk_base_y + y;
                    if (world_y <= terrain_h) break;

                    if (world_y == WATER_LEVEL) {
                        view[x, y, z] = onwater;
                    } else if (world_y < WATER_LEVEL) {
                        view[x, y, z] = Water;
                    } else {
                        view[x, y, z] = Air;
                    }
                }

                for (; y >= 0; --y) {
                    int world_y = chunk_base_y + y;
                    int depth = terrain_h - world_y;

                    if (world_y >= WATER_LEVEL) {
                        if (depth == 0) {
                            view[x, y, z] = terrain;
                        } else if (depth < 4) {
                            view[x, y, z] = subterrain;
                        } else {
                            break;
                        }
                    } else {
                        if (depth < 4) {
                            const int water_depth = WATER_LEVEL - world_y;
                            view[x, y, z] = water_depth < 6 ? underwater1 : underwater2;
                        } else {
                            break;
                        }
                    }
                }

                for (; y >= 0; --y) {
                    view[x, y, z] = Stone;
                }
            }
        }
        return chunk;
    }

    void gen(const FastNoise::SmartNode<>& generator, float* data, const glm::ivec3& chunkPos, const int seed) {
        generator->GenUniformGrid2D(
            data,
            static_cast<float>(chunkPos.x * CHUNK_SIZE.x),
            static_cast<float>(chunkPos.z * CHUNK_SIZE.z),
            CHUNK_SIZE.x,
            CHUNK_SIZE.z,
            1.f,
            1.f,
            seed
        );
    }

    std::array<float, 256> Generator::getTemperature(const glm::ivec3& chunkPos) const {
        std::array<float, 256> temp{};
        gen(m_temperature, temp.data(), chunkPos,m_seed);
        return temp;
    }

    std::array<float, 256> Generator::getHumidity(const glm::ivec3& chunkPos) const {
        std::array<float, 256> temp{};
        gen(m_humidity, temp.data(), chunkPos,m_seed);
        return temp;
    }

    std::array<float, 256> Generator::getTerrain(const glm::ivec3& chunkPos) const {
        std::array<float, 256> temp{};
        gen(m_terrain, temp.data(), chunkPos,m_seed);
        return temp;
    }

    std::array<float, 256> Generator::getTrees(const glm::ivec3& chunkPos) const {
        std::array<float, 256> temp{};
        gen(m_tree, temp.data(), chunkPos,m_seed);
        return temp;
    }

    std::array<float, 256> Generator::getDecorations(const glm::ivec3& chunkPos) const {
        std::array<float, 256> temp{};
        gen(m_decorations, temp.data(), chunkPos,m_seed);
        return temp;
    }

}
