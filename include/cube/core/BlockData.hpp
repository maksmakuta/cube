#ifndef CUBE_BLOCKDATA_HPP
#define CUBE_BLOCKDATA_HPP

#include <cstdint>
#include <string>

namespace cube {

    struct BlockData {
        uint32_t texture_top;
        uint32_t texture_side;
        uint32_t texture_bottom;
        uint32_t animation_frames;
    };

    struct BlockMetadata {
        std::string name;
        uint8_t id;
        BlockData data;
    };

}

#endif //CUBE_BLOCKDATA_HPP
