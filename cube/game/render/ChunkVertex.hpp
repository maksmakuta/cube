#ifndef CUBE_CHUNKVERTEX_HPP
#define CUBE_CHUNKVERTEX_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace cube {

    struct ChunkVertex {
        glm::u8vec3 pos;
        glm::u8vec4 nor;
        glm::u8vec2 tex;
        uint8_t      id;
    };

}

#endif //CUBE_CHUNKVERTEX_HPP