#ifndef CUBE_STATUS_HPP
#define CUBE_STATUS_HPP

#include <cstdint>

namespace cube {

    enum class Status : uint8_t {
        Unloaded,   // ready to unload
        Requested,  // checked, but no chunk found
        Empty,      // chunk exists, but empty (no voxels)
        Present,    // chunk exists and have voxels
        Meshed,     // mesh is ready to load into VRAM
        Loaded      // rendered by gpu
    };

}

#endif //CUBE_STATUS_HPP
