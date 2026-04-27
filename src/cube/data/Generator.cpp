#include <cube/data/Generator.hpp>

namespace cube {

    constexpr auto WATER_LEVEL = 160;

    Generator::Generator(const int seed) : m_seed(seed) {
        auto terrain_noise = FastNoise::New<FastNoise::Perlin>();
        terrain_noise->SetScale(256.f);
        terrain_noise->SetSeedOffset(-1);

        auto terrain_details = FastNoise::New<FastNoise::Perlin>();
        terrain_details->SetScale(16.f);
        terrain_details->SetSeedOffset(-2);

        auto terrain_fractal = FastNoise::New<FastNoise::FractalFBm>();
        terrain_fractal->SetSource(terrain_noise);
        terrain_fractal->SetWeightedStrength(terrain_details);
        terrain_fractal->SetOctaveCount(3);
        terrain_fractal->SetLacunarity(1.75);

        m_terrain_details = terrain_details;
        m_terrain = terrain_fractal;
        m_temperature = FastNoise::New<FastNoise::Simplex>();
        m_humidity = FastNoise::New<FastNoise::Simplex>();
        m_tree = FastNoise::New<FastNoise::SuperSimplex>();
        m_decorations = FastNoise::New<FastNoise::SuperSimplex>();
    }

    Chunk Generator::generate(const glm::ivec3& chunkPos) const {
        Chunk chunk;
        const auto view = chunk.view();

        const auto terrain_data = getTerrain(chunkPos);
        const auto temperature_data = getTemperature(chunkPos);
        const auto humidity_data = getHumidity(chunkPos);

        for (int x = 0; x < CHUNK_SIZE.x; x++) {
            for (int z = 0; z < CHUNK_SIZE.z; z++) {
                const auto index = x + CHUNK_SIZE.z * z;
                const auto value = terrain_data[index];
                const auto temp = temperature_data[index];
                const auto humid = humidity_data[index];
                const auto terrain = WATER_LEVEL + static_cast<int>(value * 48.f);
                for (int y = 0; y < CHUNK_SIZE.y; y++) {
                    const auto world_y = chunkPos.y * CHUNK_SIZE.y + y;

                    if (world_y <= terrain) {
                        if (world_y == terrain) {
                            view[x, y, z] = world_y+1 >= WATER_LEVEL ? GrassBlock : Sand;
                        } else if (world_y > terrain - 4) {
                            view[x, y, z] = world_y+1 >= WATER_LEVEL ? Dirt : Sand;
                        } else {
                            view[x, y, z] = Stone;
                        }
                    } else if (world_y < WATER_LEVEL) {
                        view[x, y, z] = Water;
                    } else {
                        view[x, y, z] = Air;
                    }
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

    //std::array<float, 256> Generator::getTemperature(const glm::ivec3& chunkPos) const {
    //    std::array<float, 256> temp{};
    //    gen(m_temperature, temp.data(), chunkPos,m_seed);
    //    return temp;
    //}

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