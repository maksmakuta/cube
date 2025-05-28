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
        Wood1,
        Wood2,
        Leaves1,
        Leaves2,
        Mushroom,
        Cactus,
        Weed,
        Water
    };

}

#endif //BLOCK_HPP
