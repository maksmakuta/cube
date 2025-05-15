#include "cube/generators/GeneratorHeight.hpp"

namespace cube {

    GeneratorHeight::GeneratorHeight(const int seed) : IGenerator(seed), noise(seed) {}
    GeneratorHeight::~GeneratorHeight() = default;

    Chunk GeneratorHeight::generateAt(const glm::vec2 &pos) {
        auto chunk = Chunk(pos);

        const auto offset = glm::ivec3{pos.x * CHUNK_WIDTH, 0 , pos.y * CHUNK_DEPTH};

        for (int z = 0; z < CHUNK_DEPTH; ++z) {
            for (int x = 0; x < CHUNK_WIDTH; ++x) {
                const auto fvec = glm::vec2{offset.x + x,offset.z + z};
                const auto n = ( noise.at(fvec.x,fvec.y) + 1.f) / 2.f;
                const auto height = static_cast<int>(CHUNK_HEIGHT * 0.67f + n * 32);
                for (int y = 0; y < height; ++y) {
                    auto block = BlockID::Stone;
                    if (y == 0) {
                        block = BlockID::HardStone;
                    }else if (y >= height - 1) {
                        block = BlockID::Grass;
                    }else if (y >= height - 5) {
                        block = BlockID::Dirt;
                    }
                    chunk.at({x,y,z}) = block;
                }
            }
        }

        return chunk;
    }

}