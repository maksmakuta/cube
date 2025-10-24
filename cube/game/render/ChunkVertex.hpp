#ifndef CUBE_CHUNKVERTEX_HPP
#define CUBE_CHUNKVERTEX_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace cube {

    struct ChunkVertex {
        glm::vec3 pos;
        glm::vec3 nor;
        glm::vec2 tex;
        uint32_t   id;
    };

}

#endif //CUBE_CHUNKVERTEX_HPP