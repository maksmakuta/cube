#include <cube/data/Generator.hpp>

namespace cube {

    constexpr int SEA_LEVEL = 60;

    Generator::Generator(const int seed) : m_seed(seed) {
        const auto perlin = FastNoise::New<FastNoise::Perlin>();
        m_terrainNoise = FastNoise::New<FastNoise::FractalFBm>();
        m_terrainNoise->SetSource(perlin);
        m_terrainNoise->SetOctaveCount(5);
        m_terrainNoise->SetGain(0.5f);
        m_terrainNoise->SetLacunarity(2.0f);

        m_tempNoise = FastNoise::New<FastNoise::Perlin>();
        m_moistureNoise = FastNoise::New<FastNoise::Perlin>();
    }

    Chunk Generator::generate(const glm::ivec3& chunkPos) {
        Chunk chunk;
        const auto view = chunk.view();

        const std::vector<NoiseValues> biomeData = generateNoiseValues(chunkPos);

        std::ranges::copy(biomeData, chunk.biomeData.begin());

        std::vector<float> heightMap(256);
        const auto startX = static_cast<float>(chunkPos.x * CHUNK_SIZE.x);
        const auto startZ = static_cast<float>(chunkPos.z * CHUNK_SIZE.z);
        m_terrainNoise->GenUniformGrid2D(heightMap.data(), startX, startZ, 16, 16, 1.f,1.f, m_seed);

        for (int x = 0; x < 16; ++x) {
            for (int z = 0; z < 16; ++z) {
                const int index2D = x + (z * 16);
                const int surfaceHeight = static_cast<int>((heightMap[index2D] + 1.0f) * 0.5f * 60.0f) + 40;
                const float temp = biomeData[index2D].temp;
                const float moist = biomeData[index2D].moisture;
                const int startY = chunkPos.y * CHUNK_SIZE.y;

                for (int y = 0; y < 16; ++y) {
                    if (const int worldY = startY + y; worldY <= surfaceHeight) {
                        view[x, y, z] = getBlockForPosition(worldY, surfaceHeight, temp, moist);
                    } else if (worldY <= SEA_LEVEL) {
                        view[x, y, z] = Water;
                    }
                }
            }
        }

        return chunk;
    }

    std::vector<NoiseValues> Generator::generateNoiseValues(const glm::ivec3 &chunkPos) {
        std::vector<NoiseValues> values(256);

        std::vector<float> tempMap(256);
        std::vector<float> moistureMap(256);

        const auto startX = static_cast<float>(chunkPos.x * CHUNK_SIZE.x);
        const auto startZ = static_cast<float>(chunkPos.z * CHUNK_SIZE.z);

        m_tempNoise->GenUniformGrid2D(tempMap.data(), startX, startZ, 16, 16, 0.5f,0.5f, m_seed + 1);
        m_moistureNoise->GenUniformGrid2D(moistureMap.data(), startX, startZ, 16, 16, 0.5f, 0.5f,m_seed + 2);

        for (int i = 0; i < 256; ++i) {
            values[i] = {tempMap[i], moistureMap[i]};
        }

        return values;
    }

   Block Generator::getBlockForPosition(const int y, const int surfaceHeight, const float temp, const float moisture) {

    if (y < surfaceHeight - 4) {
        if (y < 20) return Tuff;
        return Stone;
    }

    const bool isUnderwater = surfaceHeight < SEA_LEVEL;

    if (y < surfaceHeight) {
        if (temp > 0.3f && moisture < -0.2f) return Sand;

        if (isUnderwater) {
            return moisture > 0.2f ? Clay : Gravel;
        }

        return Dirt;
    }

    if (y == surfaceHeight) {

        if (isUnderwater || y == SEA_LEVEL) {
            if (temp < -0.4f) return Ice;
            if (temp > 0.1f && moisture < 0.0f) return Sand;
            if (moisture > 0.3f) return Clay;
            return Gravel;
        }

        if (temp < -0.3f) {
            return moisture > 0.2f ? Snow : SnowDirt;
        }

        if (temp > 0.3f) {
            if (moisture < -0.2f) return Sand;
            return GrassBlock;
        }

        return GrassBlock;
    }

    return Air;
}

}
