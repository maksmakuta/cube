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
                const auto y = static_cast<int>(CHUNK_HEIGHT * 0.67f + n * 32);

                chunk.at({x,y,z}) = BlockID::Wood;
            }
        }

        return chunk;
    }

}