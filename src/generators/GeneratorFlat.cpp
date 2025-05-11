#include "cube/generators/GeneratorFlat.hpp"

namespace cube {

    GeneratorFlat::GeneratorFlat() = default;
    GeneratorFlat::~GeneratorFlat() = default;

    Chunk GeneratorFlat::generateAt(const glm::vec2 &pos) {
        const auto layout = std::vector{BlockID::HardStone, BlockID::Stone, BlockID::Dirt, BlockID::Grass};
        Chunk temp;
        for (int y = 0; y <= 3; ++y) {
            for (int z = 0; z < CHUNK_DEPTH; ++z) {
                for (int x = 0; x < CHUNK_WIDTH; ++x) {
                    temp.at({x,y,z}) = layout[y];
                }
            }
        }
        return temp;
    }

}