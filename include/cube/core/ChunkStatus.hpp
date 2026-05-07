#ifndef CUBE_CHUNKSTATUS_HPP
#define CUBE_CHUNKSTATUS_HPP

namespace cube {

    enum class ChunkStatus {
        Unknown,        // no chunk found
        Empty,          // new chunk, no data
        QueuedGen,      // data is generated soon
        GenerateData,   // generating
        Generated,      // data is present
        QueuedMesh,     // data is meshed soon
        Mesh,           // meshing
        Meshed,         // mesh is done
        Active,         // ready to render
        Unload          // need to be removed
    };

}

#endif //CUBE_CHUNKSTATUS_HPP
