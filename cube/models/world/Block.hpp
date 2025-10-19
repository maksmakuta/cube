#ifndef CUBE_BLOCK_HPP
#define CUBE_BLOCK_HPP

#include <cstdint>
#include <string>

namespace cube {

    using BlockID = uint8_t;

    struct BlockMetadata {
        int8_t top;
        int8_t side;
        int8_t bottom;
    };

    enum class Block : BlockID {
        Air         = 0,
        Grass       = 1,
        Dirt        = 2,
        Stone       = 3,
        Wood        = 4,
        Leaves      = 5,
        Bushes      = 32,
        Flower      = 33,
        Water       = 128,
    };

    std::string toString(const Block&);
    BlockMetadata getMetadata(const Block&);

}

#endif //CUBE_BLOCK_HPP