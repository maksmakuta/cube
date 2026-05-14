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
        int16_t texture_overlay; //apply only on side
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

        Water,
        Lava,

        Bedrock,

        COUNT
    };

    BlockInfo getInfo(const Block& block);

}

#endif //CUBE_BLOCK_HPP
