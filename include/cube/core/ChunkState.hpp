#ifndef CUBE_CHUNKSTATE_HPP
#define CUBE_CHUNKSTATE_HPP

namespace cube {

    enum class ChunkState {
        Requested,         // Asked to generate
        Generating,        // Currently in a background thread
        Generated,         // Raw data exists in World, awaiting neighbors
        Meshing,           // Currently being meshed in a background thread
        Active             // Uploaded to GPU
    };

}

#endif //CUBE_CHUNKSTATE_HPP
