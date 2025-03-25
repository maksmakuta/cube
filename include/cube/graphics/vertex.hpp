#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace cube::graphics {

    /*
     *  bits: 32                                       0
     *          0000 0000 vvvv uuuu yyyy yyyy zzzz xxxx
     */
    using Vertex = uint32_t;

    inline Vertex pack(const glm::vec3 &pos, const glm::vec2 &uv) {
        Vertex result = 0;
        result |= static_cast<uint32_t>(pos.x) & 0xF;
        result |= (static_cast<uint32_t>(pos.z) & 0xF ) << 4;
        result |= (static_cast<uint32_t>(pos.y) & 0xFF) << 8;
        result |= (static_cast<uint32_t>(uv.x * 16.f) & 0xF ) << 16;
        result |= (static_cast<uint32_t>(uv.y * 16.f) & 0xF ) << 20;
        return result;
    }

}

#endif //VERTEX_HPP
