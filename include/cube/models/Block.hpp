#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <cstdint>

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
}

#endif //BLOCK_HPP
