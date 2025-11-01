#ifndef CUBE_BLOCK_H
#define CUBE_BLOCK_H

#include <cstdint>

namespace cube{

    enum class Block{
        Air,

        Grass,
        Dirt,
        Sand,
        Gravel,

        Stone,
        Diorite,
        Granite,

        GoldOre,
        CoalOre,
        IronOre,

        BirchLog,
        OakLog,
        SpruceLog,
        Leaves,

        Water,
        Ice,

        ThinBush,
        ThickBush,

        RedFlower,
        YellowFlower,
        BlueFlower,
    };

    struct BlockMaterial{
        uint8_t top;
        uint8_t side;
        uint8_t bottom;
    };

    BlockMaterial getBlockMaterial(const Block& block);
}



#endif //CUBE_BLOCK_H