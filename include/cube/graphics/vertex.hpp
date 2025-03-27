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

    inline void unpack(const Vertex& vertex) { //used for tests, maybe removed in future
        const auto px = static_cast<float>( vertex & 0x000Fu);
        const auto pz = static_cast<float>((vertex >> 4) & 0x000Fu);
        const auto py = static_cast<float>((vertex >> 8) & 0x00FFu);
        const auto tu = static_cast<float>((vertex >> 16) & 0x000Fu) / 15.f;
        const auto tv = static_cast<float>((vertex >> 20) & 0x000Fu) / 15.f;
        std::printf("pos: [%2.2f,%2.2f,%2.2f] tex: [%2.2f,%2.2f]\n",px,py,pz,tu,tv);
    }

    inline Vertex pack(const glm::vec3 &pos, const glm::vec2 &uv) {
        Vertex result = 0;
        result |= static_cast<uint32_t>(pos.x) & 0xF;
        result |= (static_cast<uint32_t>(pos.z) & 0xF ) << 4;
        result |= (static_cast<uint32_t>(pos.y) & 0xFF) << 8;
        result |= (static_cast<uint32_t>(uv.x * 15.f) & 0xF ) << 16;
        result |= (static_cast<uint32_t>(uv.y * 15.f) & 0xF ) << 20;
        return result;
    }

}

#endif //VERTEX_HPP
