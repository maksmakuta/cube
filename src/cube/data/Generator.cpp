#include <cube/data/Generator.hpp>

namespace cube {

    constexpr auto BASELINE = 60;

    Generator::Generator(const int seed) : m_seed(seed) {}

    Chunk Generator::generate(const glm::ivec3& chunkPos) {
        Chunk chunk;
        const auto biome_data = generateNoiseValues(chunkPos);
        std::copy(biome_data.begin(), biome_data.end(), chunk.biomeData.begin());

        auto view = chunk.view();

        for (int x = 0; x < CHUNK_SIZE.x; x++) {
            for (int z = 0; z < CHUNK_SIZE.z; z++) {
                for (int y = 0; y < CHUNK_SIZE.y; y++) {
                    const int world_y = chunkPos.y * CHUNK_SIZE.y + y;

                    if (world_y > BASELINE) {
                        view[x, y, z] = Air;
                    } else if (world_y == BASELINE) {
                        view[x, y, z] = GrassBlock;
                    } else if (world_y >= BASELINE - 3) {
                        view[x, y, z] = Dirt;
                    } else {
                        view[x, y, z] = Stone;
                    }
                }
            }
        }

        return chunk;
    }

    std::vector<NoiseValues> Generator::generateNoiseValues(const glm::ivec3& chunkPos) {
        return std::vector<NoiseValues>(256,{0,0});
    }


}