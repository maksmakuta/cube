#ifndef CUBE_CHUNK_H
#define CUBE_CHUNK_H

#include <memory>
#include <glm/vec3.hpp>

namespace cube{

    constexpr auto CHUNK_SIZE = glm::ivec3(16,128,16);

    class Chunk{
    public:
        Chunk();
    };

    using ChunkPtr = std::shared_ptr<Chunk>;

}



#endif //CUBE_CHUNK_H