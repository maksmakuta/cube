#ifndef CUBE_BLOCK_HPP
#define CUBE_BLOCK_HPP

#include <cstdint>
#include <string_view>

namespace cube {

    enum Block : uint8_t {
        Air = 0,
        GrassBlock,
        Dirt,
        SnowDirt,
        Snow,
        Stone,
        Sand,
        Clay,
        Gravel,
        Diorite,
        Tuff,
        Ice,

        OreCoal,
        OreCopper,
        OreIron,
        OreGold,

        CoralBubble,

        BirchLeaves,
        BirchLog,
        JungleLeaves,
        JungleLog,
        OakLeaves,
        OakLog,
        SpruceLeaves,
        SpruceLog,

        Melon,
        Pumpkin,
        Cactus,
        CactusFlower,

        MushroomBrown,
        MushroomRed,

        Fern,
        Bush,
        Grass,
        Tallgrass,
        Sugarcane,
        Kelp,

        Dandelion,
        Poppy,
        Tulip,

        Vine,
        Water,

        COUNT
    };

    enum class BlockShape : uint8_t {
        Cube,
        Cross,
        Panel,
        Liquid
    };

    struct BlockData {
        uint8_t top;
        uint8_t side;
        uint8_t bottom;
        uint8_t count;
        int8_t  sideOverlay;
        bool isTinted;
        BlockShape shape;
    };

    struct BlockInfo {
        std::string_view name;
        BlockData data;
    };

    const BlockInfo& getBlockInfo(Block block);
    BlockData getBlockData(Block block);

}

#endif //CUBE_BLOCK_HPP
