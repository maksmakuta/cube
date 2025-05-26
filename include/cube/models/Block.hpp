#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <cstdint>
#include <glm/vec3.hpp>

namespace cube {

    enum class BlockID : uint8_t{
        Air,
        Grass,
        Dirt,
        Stone,
        Wood,
        Leaves,
        Bush,
        Flower,
        HardStone
    };

    glm::vec3 getColor(BlockID);
}

#endif //BLOCK_HPP
