#include <cube/data/Generator.hpp>

namespace cube {

    constexpr auto WATER_LEVEL = 72;

    Generator::Generator(const int seed) : m_seed(seed) {
        const auto base = FastNoise::New<FastNoise::Perlin>();

        m_noise = FastNoise::New<FastNoise::FractalFBm>();
        m_noise->SetSource(base);
        m_noise->SetOctaveCount(4);
        m_noise->SetLacunarity(2.0f);
        m_noise->SetGain(0.5f);

        const auto tbase = FastNoise::New<FastNoise::Simplex>();
        m_temp = FastNoise::New<FastNoise::FractalFBm>();
        m_temp->SetSource(tbase);
        m_temp->SetOctaveCount(2);
        m_temp->SetLacunarity(1.99f);
        m_temp->SetGain(0.3f);

        const auto hbase = FastNoise::New<FastNoise::Simplex>();
        m_humid = FastNoise::New<FastNoise::FractalFBm>();
        m_humid->SetSource(hbase);
        m_humid->SetOctaveCount(2);
        m_humid->SetLacunarity(1.66f);
        m_humid->SetGain(0.3f);
    }

    Chunk Generator::generate(const glm::ivec3& chunkPos) {
        Chunk chunk;
        const auto view = chunk.view();

        const auto biome_data = generateNoiseValues(chunkPos);
        std::ranges::copy(biome_data, chunk.biomeData.begin());

        std::array<float, 256> data{};
        std::array<float, 256> decorations{};

        const auto pos_x = static_cast<float>(chunkPos.x * CHUNK_SIZE.x);
        const auto pos_z = static_cast<float>(chunkPos.z * CHUNK_SIZE.z);

        m_noise->GenUniformGrid2D(data.begin(), pos_x, pos_z, 16, 16, 1.0f, 1.0f, m_seed);
        m_noise->GenUniformGrid2D(decorations.begin(), pos_x, pos_z, 16, 16, 16.0f, 16.0f, m_seed * m_seed);

        for (int x = 0; x < CHUNK_SIZE.x; x++) {
            for (int z = 0; z < CHUNK_SIZE.z; z++) {

                const int index = z * CHUNK_SIZE.x + x;
                const int terrain = static_cast<int>((data[index] + 1.0f) / 2.f * 48.0f) + 64;

                for (int y = 0; y < CHUNK_SIZE.y; y++) {

                    const int worldY = chunkPos.y * CHUNK_SIZE.y + y;

                    if (worldY < terrain - 4) {
                        view[x, y, z] = Stone;
                    }
                    else if (worldY < terrain) {
                        if (biome_data[index].temp > 0.3) {
                            view[x, y, z] = Sand;
                        } else {
                            view[x, y, z] = (worldY > WATER_LEVEL) ? Dirt : Sand;
                        }
                    }
                    else if (worldY == terrain) {
                        if (biome_data[index].temp < -0.3) {
                            view[x, y, z] = SnowDirt;
                        } else if (biome_data[index].temp > 0.3) {
                            view[x, y, z] = Sand;
                        } else {
                            view[x, y, z] = (worldY > WATER_LEVEL) ? GrassBlock : Sand;
                        }
                    }
                    else if (worldY == terrain + 1 && terrain > WATER_LEVEL) {
                        const float d = decorations[index];

                        if (biome_data[index].temp > 0.3) {
                            if (d > 0.9f) {
                                view[x, y, z] = Cactus;
                            }
                            if (y + 1 < CHUNK_SIZE.y && d > 0.98f) {
                                view[x, y+1, z] = CactusFlower;
                            }
                        }else if (biome_data[index].temp < -0.3) {
                            if (d > 0.1f && d < 0.15f) {
                                view[x, y, z] = Fern;
                            }
                            else if (d < -0.9f) {
                                view[x, y, z] = Grass;
                            }
                        }else {
                            if (d > 0.25f && d < 0.3f && biome_data[index].moisture >= 0.6) {
                                if (d > 0.275f) {
                                    view[x, y, z] = Pumpkin;
                                }else {
                                    view[x, y, z] = Melon;
                                }
                            }
                        }
                    }
                    else if (worldY <= WATER_LEVEL) {
                        if (biome_data[index].temp < -0.3 && worldY == WATER_LEVEL) {
                            view[x, y, z] = Ice;
                        }else {
                            view[x, y, z] = Water;
                        }
                    }
                    else {
                        view[x, y, z] = Air;
                    }
                }
            }
        }

        return chunk;
    }

    std::vector<NoiseValues> Generator::generateNoiseValues(const glm::ivec3& chunkPos) {
        std::array<float, 256> temperature{};
        std::array<float, 256> humidity{};

        const auto x_offset = static_cast<float>(chunkPos.x * CHUNK_SIZE.x);
        const auto z_offset = static_cast<float>(chunkPos.z * CHUNK_SIZE.z);

        m_temp->GenUniformGrid2D(temperature.data(), x_offset, z_offset,16, 16, 1.0f, 1.0f, m_seed * 2);
        m_humid->GenUniformGrid2D(humidity.data(), x_offset, z_offset, 16, 16, 1.0f, 1.0f, m_seed / 2);

        std::vector<NoiseValues> noiseValues;
        noiseValues.reserve(256);

        for (auto i = 0; i < 256; i++) {
            noiseValues.emplace_back(temperature[i], humidity[i]);
        }

        return noiseValues;
    }


}