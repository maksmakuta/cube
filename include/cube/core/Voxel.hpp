#ifndef CUBE_VOXEL_HPP
#define CUBE_VOXEL_HPP

#include <cstdint>

namespace cube {

    // Voxel contains block id (uint8_t) + useful flags that stored in Chunk container class
    using Voxel = uint16_t;

}

#endif //CUBE_VOXEL_HPP
