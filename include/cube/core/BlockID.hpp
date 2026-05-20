#ifndef CUBE_BLOCKID_HPP
#define CUBE_BLOCKID_HPP

#include <cstdint>

namespace cube {

    enum class MeshType {
        Cube,
        Cross,
        Panel,
        Custom
    };

    enum BlockID : uint8_t {
        Air,

        SnowGrassBlock,
        GrassBlock,
        CoarseDirt,
        RedSand,
        Gravel,
        Clay,
        Dirt,
        Sand,
        Mud,

        Amethyst,
        Andesite,
        Basalt,
        Bedrock,
        BlackStone,
        Calcite,
        CobbleStone,
        Deepslate,
        Diorite,
        Dripstone,
        Granite,
        RedSandStone,
        SandStone,
        Stone,
        Tuff,

        Ice,
        BlueIce,
        PackedIce,

        AcaciaLeaves,
        BirchLeaves,
        CherryLeaves,
        DarkOakLeaves,
        JungleLeaves,
        OakLeaves,
        PaleOakLeaves,
        SpruceLeaves,

        AcaciaLog,
        BirchLog,
        CherryLog,
        DarkOakLog,
        JungleLog,
        OakLog,
        PaleOakLog,
        SpruceLog,

        AcaciaSapling,
        BirchSapling,
        CherrySapling,
        DarkOakSapling,
        JungleSapling,
        OakSapling,
        PaleOakSapling,
        SpruceSapling,

        Allium,
        AzureBluet,
        BlueOrchid,
        CornFlower,
        Dandelion,
        OrangeTulip,
        OxeyeDaisy,
        PinkTulip,
        Poppy,
        RedTulip,
        WhiteTulip,

        BrainCoralBlock,
        BubbleCoralBlock,
        FireCoralBlock,
        HornCoralBlock,
        TubeCoralBlock,

        DeadBrainCoralBlock,
        DeadBubbleCoralBlock,
        DeadFireCoralBlock,
        DeadHornCoralBlock,
        DeadTubeCoralBlock,

        BrainCoral,
        BubbleCoral,
        FireCoral,
        HornCoral,
        TubeCoral,

        DeadBrainCoral,
        DeadBubbleCoral,
        DeadFireCoral,
        DeadHornCoral,
        DeadTubeCoral,

        Bush,
        DeadBush,
        Fern,
        Grass,
        ShortDryGrass,
        ShortGrass,
        TallDryGrass,
        SugarCane,
        SweetBerry,

        CaveVines,
        CaveVinesLit,
        CavePlant,
        CavePlantLit,

        Cactus,
        CactusFlower,

        BrownMushroom,
        RedMushroom,

        Pumpkin,
        Melon,

        LargeFern,
        LargeFernTop,
        Peony,
        PeonyTop,
        RoseBush,
        RoseBushTop,
        TallGrass,
        TallGrassTop,
        TallSeaGrass,
        TallSeaGrassTop,

        Kelp,
        Seagrass,

        Water,
        Lava,

        COUNT
    };

    struct BlockInfo {
        uint8_t top;
        uint8_t side;
        uint8_t bottom;
        uint8_t frames;
        MeshType type;
        bool transparent;
    };

    BlockInfo getBlockInfo(BlockID);

}

#endif //CUBE_BLOCKID_HPP
