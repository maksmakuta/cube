#ifndef CUBE_VERTEX_HPP
#define CUBE_VERTEX_HPP

#include <cstdint>

namespace cube {

    struct Vertex final {
        uint64_t data;

        Vertex(
            const uint8_t x, const uint8_t y, const uint8_t z,  // position
            const uint8_t n,                                    // normals
            const uint8_t u, const uint8_t v,                   // tex coordinates
            const uint8_t t, const uint8_t o,                   // texture id and overlay
            const uint8_t r, const uint8_t g, const uint8_t b,  // tint color
            const uint8_t a, const uint8_t l                    // AO and global lighting
        ) : data(0) {
            data |=  static_cast<uint64_t>(x)     & 0x1F;
            data |= (static_cast<uint64_t>(y)     & 0x1F)  << 5;
            data |= (static_cast<uint64_t>(z)     & 0x1F)  << 10;
            data |= (static_cast<uint64_t>(n)     & 0x07)  << 15;
            data |= (static_cast<uint64_t>(u)     & 0x0F)  << 18;
            data |= (static_cast<uint64_t>(v)     & 0x0F)  << 22;
            data |= (static_cast<uint64_t>(t)     & 0x0F)  << 26;
            data |= (static_cast<uint64_t>(o)     & 0x0F)  << 30;
            data |= (static_cast<uint64_t>(r)     & 0xFF)  << 34;
            data |= (static_cast<uint64_t>(g)     & 0xFF)  << 42;
            data |= (static_cast<uint64_t>(b)     & 0xFF)  << 50;
            data |= (static_cast<uint64_t>(a)     & 0x03)  << 58;
            data |= (static_cast<uint64_t>(l)     & 0x0F)  << 60;
        }
    };

}

#endif //CUBE_VERTEX_HPP
