#ifndef CUBE_BLOCK_HPP
#define CUBE_BLOCK_HPP

#include <glm/fwd.hpp>

namespace cube {

    enum Block : uint8_t {
        Air = 0,
        Stone,
        Dirt,
        GrassBlock,
        Sand,
        Gravel,
        Snow,
        Ice,
        Clay,
        Tuff,
        Diorite,

        CoalOre,
        IronOre,
        CopperOre,
        GoldOre,

        OakLog,
        OakLeaves,
        BirchLog,
        BirchLeaves,
        SpruceLog,
        SpruceLeaves,
        JungleLog,
        JungleLeaves,

        Bush,
        Fern,
        Poppy,
        Dandelion,
        RedTulip,
        RedMushroom,
        BrownMushroom,
        SugarCane,
        TallGrass,
        Vine,
        BubbleCoral,

        Cactus,

        Pumpkin,
        Melon,

        Water,
        Kelp,

        Count
    };

}

#endif //CUBE_BLOCK_HPP
