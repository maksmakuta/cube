#ifndef CUBE_BLOCK_HPP
#define CUBE_BLOCK_HPP

#include <cstdint>

namespace cube {

    enum class Block : uint8_t {
        Air = 0,

        Grass,
        Dirt,
        Stone,

        Water,
        Lava,

        Bedrock = 255,
    };

}

#endif //CUBE_BLOCK_HPP
