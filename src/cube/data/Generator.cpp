#include "cube/data/Generator.hpp"

#include "cube/core/BlockID.hpp"

namespace cube {

    Generator::Generator(const int seed) : m_seed(seed) {}

    std::shared_ptr<Chunk> Generator::generate(const glm::ivec3& pos) {
        auto chunk = std::make_shared<Chunk>();

        if (pos.y < 0 || pos.y > 8) {
            return chunk;
        }

        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                for (int y = 0; y < CHUNK_SIZE; y++) {
                    const auto world_y = pos.y * CHUNK_SIZE + y;
                    auto voxel = Air;
                    if (world_y == 0) {
                        voxel = Bedrock;
                    } else if (world_y < 8) {
                        voxel = Deepslate;
                    } else if (world_y < 28) {
                        voxel = Stone;
                    } else if (world_y < 32) {
                        voxel = Dirt;
                    } else if (world_y == 32) {
                        voxel = GrassBlock;
                    } else {
                        continue;
                    }

                    chunk->at({x,y,z}) = voxel;
                }
            }
        }

        return chunk;
    }

    int Generator::getHeightAt(const glm::ivec3&) {
        return 32;
    }

}
