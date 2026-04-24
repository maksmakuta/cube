#include <cube/data/Generator.hpp>

namespace cube {

    Generator::Generator(const int seed) : m_seed(seed) {
        const auto perlin = FastNoise::New<FastNoise::Perlin>();
        m_terrainNoise = FastNoise::New<FastNoise::FractalFBm>();
        m_terrainNoise->SetSource(perlin);
        m_terrainNoise->SetOctaveCount(5);
        m_terrainNoise->SetGain(0.5f);
        m_terrainNoise->SetLacunarity(2.0f);

        m_tempNoise = FastNoise::New<FastNoise::Perlin>();
    }

    Chunk Generator::generate(const glm::ivec3 &chunkPos) {
        Chunk chunk;
        const auto view = chunk.view();

        std::vector<float> heightMap(CHUNK_SIZE.x * CHUNK_SIZE.z);
        std::vector<float> tempMap(CHUNK_SIZE.x * CHUNK_SIZE.z);

        m_terrainNoise->GenUniformGrid2D(heightMap.data(),
                                         chunkPos.x * CHUNK_SIZE.x, chunkPos.z * CHUNK_SIZE.z,
                                         CHUNK_SIZE.x, CHUNK_SIZE.z, 1.f, 1.f, m_seed);

        m_tempNoise->GenUniformGrid2D(tempMap.data(),
                                      chunkPos.x * CHUNK_SIZE.x, chunkPos.z * CHUNK_SIZE.z,
                                      CHUNK_SIZE.x, CHUNK_SIZE.z, 0.001f, 0.001f, m_seed + 1);

        for (int z = 0; z < CHUNK_SIZE.z; ++z) {
            for (int x = 0; x < CHUNK_SIZE.x; ++x) {
                const float rawH = heightMap[z * CHUNK_SIZE.x + x];
                const float rawT = tempMap[z * CHUNK_SIZE.x + x];
                const int terrainHeight = static_cast<int>(128.0f + (rawH * 60.0f));
                const float altitudeEffect = (terrainHeight - 128.0f) / 128.0f;
                const float finalTemp = rawT - altitudeEffect;

                for (int y = 0; y < CHUNK_SIZE.y; ++y) {
                    const int worldY = (chunkPos.y * CHUNK_SIZE.y) + y;
                    view[x, y, z] = getBlockForPosition({x, worldY, z}, terrainHeight, finalTemp);
                }
            }
        }
        return chunk;
    }

    Block Generator::getBlockForPosition(const glm::ivec3 &pos, const int height, const float temp) {
        if (pos.y > height) return Air;

        if (temp < -0.2f) {
            if (pos.y == height) return Stone;
            return Stone;
        }

        if (temp > 0.4f || height < 110) {
            if (pos.y == height) return Sand;
            return Dirt;
        }

        if (pos.y == height) return Grass;
        if (pos.y > height - 4) return Dirt;

        return Stone;
    }

}
