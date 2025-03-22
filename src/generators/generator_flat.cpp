#include "cube/generators/generator_flat.hpp"

#include "cube/core/block.hpp"
#include "cube/core/constants.hpp"

namespace cube::generators {

    GeneratorFlat::GeneratorFlat() : IGenerator() {}

    void GeneratorFlat::seed(const Seed&){
        //not use
    }

    BlockData GeneratorFlat::at(uint8_t x, uint8_t y, uint8_t z) {
        if (y == 0) {
            return static_cast<BlockData>(BlockID::Bedrock);
        }

        if (y < CHUNK_HEIGHT / 2) {
            static_cast<BlockData>(BlockID::Stone);
        }

        if (y == CHUNK_HEIGHT / 2) {
            static_cast<BlockData>(BlockID::Dirt);
        }

        if (y == CHUNK_HEIGHT / 2 + 1) {
            static_cast<BlockData>(BlockID::Grass);
        }

        return static_cast<BlockData>(BlockID::Air);
    }

}
