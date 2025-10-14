#ifndef CUBE_BLOCK_HPP
#define CUBE_BLOCK_HPP

#include <cstdint>

namespace cube {

    using BlockID = uint8_t;

    enum class Block : BlockID {
        Air = 0,
        Grass = 1,
        Dirt = 2,
        Stone = 3,
        Wood = 4,
        Leaves = 5,
        Water = 6,
        Bushes = 7,
        Flower = 8,
    };

    constexpr auto BLOCK_AIR = 0;

}

#endif //CUBE_BLOCK_HPP