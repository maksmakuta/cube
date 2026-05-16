#ifndef CUBE_BLOCK_HPP
#define CUBE_BLOCK_HPP

#include <cstdint>

namespace cube {

    enum class BlockType {
        Cube,
        Cross,
        VPanel,
        HPanel,
        Flowing
    };

    struct BlockInfo final {
        int16_t texture_top;
        int16_t texture_side;
        int16_t texture_bottom;
        int16_t anim_count;
        int16_t anim_speed;
        bool is_tint;
        BlockType type;
    };

    enum class Block : uint8_t {
        Air = 0,

        Grass,
        Dirt,
        Sand,
        RedSand,
        Gravel,
        Mud,
        Clay,

        Stone,
        Andesite,
        Basalt,
        Calcite,
        Deepslate,
        Diorite,
        Granite,
        SandStone,
        RedSandStone,
        Tuff,

        Ice,
        BlueIce,
        PackedIce,

        Cactus,
        CactusFlower,

        BrainCoralBlock,
        BubbleCoralBlock,
        FireCoralBlock,
        HornCoralBlock,
        TubeCoralBlock,
        BrainFan,
        BubbleFan,
        FireFan,
        HornFan,
        TubeFan,
        BrainCoral,
        BubbleCoral,
        FireCoral,
        HornCoral,
        TubeCoral,
        BrainDead,
        BubbleDead,
        FireDead,
        HornDead,
        TubeDead,

        Bush,
        DeadBush,
        FireflyBush,
        CaveVines,
        CaveVinesLit,
        CaveVinesPlant,
        CaveVinesPlantLit,
        HangingRoots,
        ShortDryGrass,
        ShortGrass,
        SugarCane,
        SweetBerryBush0,
        SweetBerryBush1,
        SweetBerryBush2,
        SweetBerryBush3,
        TallDryGrass,

        Allium,
        AzureBluet,
        BlueOrchid,
        Cornflower,
        Dandelion,
        OrangeTulip,
        OxeyeDaisy,
        PinkTulip,
        Poppy,
        RedTulip,
        WhiteTulip,

        AcaciaLeaves,
        AzaleaLeaves,
        BirchLeaves,
        CherryLeaves,
        DarkOakLeaves,
        JungleLeaves,
        OakLeaves,
        PaleOakLeaves,
        SpruceLeaves,

        AcaciaLog,
        AzaleaLog,
        BirchLog,
        CherryLog,
        DarkOakLog,
        JungleLog,
        OakLog,
        PaleOakLog,
        SpruceLog,

        AcaciaSapling,
        AzaleaSapling,
        FloweringAzaleaSapling,
        BirchSapling,
        CherrySapling,
        DarkOakSapling,
        JungleSapling,
        OakSapling,
        PaleOakSapling,
        SpruceSapling,

        LargeFernB,
        LargeFernT,
        PeonyB,
        PeonyT,
        RoseBushB,
        RoseBushT,
        TallGrassB,
        TallGrassT,
        TallSeagrassB,
        TallSeagrassT,

        BrownMushroom,
        RedMushroom,

        Kelp,
        Seagrass,

        Melon,
        Pumpkin,

        Water,
        Lava,

        Bedrock,

        COUNT
    };

    BlockInfo getInfo(const Block& block);

}

#endif //CUBE_BLOCK_HPP
