#ifndef CUBE_BLOCK_HPP
#define CUBE_BLOCK_HPP

#include <cstdint>
#include <string_view>

namespace cube {

    enum Block : uint8_t {
        Air = 0,
        Grass,
        Dirt,
        Stone,
        Sand,

        COUNT
    };

    struct BlockData {
        uint8_t top;
        uint8_t side;
        uint8_t bottom;
        uint8_t count;
    };

    struct BlockInfo {
        std::string_view name;
        BlockData data;
    };

    const BlockInfo& getBlockInfo(Block block);
    BlockData getBlockData(Block block);

}

#endif //CUBE_BLOCK_HPP
