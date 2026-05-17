#ifndef CUBE_BLOCKID_HPP
#define CUBE_BLOCKID_HPP

#include <cstdint>

namespace cube {

    enum BlockID : uint8_t {
        Air,

        Grass,
        Dirt,
        Stone,
        HardStone,
        Sand,
        Gravel,

        Leaves,
        Wood,
        Sapling,

        Bush,
        DeadBush,
        Fern,
        FlowerYellow,
        FlowerRed,
        FlowerWhite,

        Cane,
        Melon,
        Pumpkin,
        Cacti,
        CactiFlower,

        Water,
        Lava,

        COUNT
    };

}

#endif //CUBE_BLOCKID_HPP
