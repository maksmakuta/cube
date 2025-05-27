#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <cstdint>

namespace cube {

    enum class BlockID : uint8_t{
        Air,
        Grass,
        Dirt,
        Sand,
        Stone,
        Graystone,
        Wood,
        Leaves,
        Mushroom,
        Cactus,
        Water
    };

}

#endif //BLOCK_HPP
