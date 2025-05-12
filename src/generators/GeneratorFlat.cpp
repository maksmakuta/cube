#include "cube/generators/GeneratorFlat.hpp"

namespace cube {

    GeneratorFlat::GeneratorFlat() = default;
    GeneratorFlat::~GeneratorFlat() = default;

    std::shared_ptr<Chunk> GeneratorFlat::generateAt(const glm::vec2 &pos) {
        const auto layout = std::vector{
            BlockID::HardStone,
            BlockID::Stone,
            BlockID::Stone,
            BlockID::Stone,
            BlockID::Grass
        };

        const auto temp = std::make_shared<Chunk>();
        for (int y = 0; y < layout.size(); ++y) {
            for (int z = 0; z < CHUNK_DEPTH; ++z) {
                for (int x = 0; x < CHUNK_WIDTH; ++x) {
                    temp->at({x,y,z}) = layout[y];
                }
            }
        }
        return temp;
    }

}