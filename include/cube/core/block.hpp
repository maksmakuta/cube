#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <cstdint>

namespace cube {

    enum class BlockID : uint8_t{
        Air = 0,
        Grass = 1,
        Dirt = 2,
        Stone = 3,

        Bedrock = 255
    };

}

#endif //BLOCK_HPP
