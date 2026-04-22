#ifndef CUBE_BLOCK_HPP
#define CUBE_BLOCK_HPP

#include <cstdint>
#include <string>

namespace cube {

    enum Block : uint8_t {
        Air = 0,
        Grass,
        Dirt,
        Stone
    };

    struct BlockData {
        uint8_t top;
        uint8_t side;
        uint8_t bottom;
        uint8_t count;
    };

    struct BlockInfo {
        std::string name;
        BlockData data;
    };

    BlockInfo getBlockInfo(const Block& block);
    BlockData getBlockData(const Block& block);

}

#endif //CUBE_BLOCK_HPP
