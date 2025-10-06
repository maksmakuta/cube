#ifndef CUBE_BLOCK_HPP
#define CUBE_BLOCK_HPP
#include <cstdint>

namespace cube {

    enum class Block {
        Air     = 0,
        Grass   = 1,
        Dirt    = 2,
        Stone   = 3,
        Wood    = 4,
        Leaves  = 5,
        Water   = 6,
        Bush    = 7,
        Flower  = 8,
    };

    enum class BlockType {
        Empty,
        Fill,       // []
        Cross,      // X
        Sharp       // #
    };

    struct BlockInfo {
        BlockType type;
        int8_t top;
        int8_t side;
        int8_t bottom;
    };

    BlockInfo getBlockInfo(const Block& block);

}

#endif //CUBE_BLOCK_HPP