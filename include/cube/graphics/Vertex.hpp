#ifndef CUBE_VERTEX_HPP
#define CUBE_VERTEX_HPP

#include <cstdint>

namespace cube {

    struct Vertex final {
        uint64_t data;

        Vertex(
            const uint8_t x, const uint8_t y, const uint8_t z,
            const uint8_t n,
            const uint8_t u, const uint8_t v,
            const uint16_t texID,
            const uint8_t animFrames,
            const uint8_t animSpeed,
            const uint8_t overlay,
            const uint8_t r, const uint8_t g, const uint8_t b,
            const uint8_t ao
        ) : data(0) {

            uint32_t low = 0;
            low |= (x & 0x1Fu);
            low |= (y & 0x1Fu) << 5u;
            low |= (z & 0x1Fu) << 10u;
            low |= (n & 0x07u) << 15u;
            low |= (u & 0x03u) << 18u;
            low |= (v & 0x03u) << 20u;
            low |= (ao & 0x03u) << 22u;
            low |= (overlay & 0x0Fu) << 24u;
            low |= (animSpeed & 0x07u) << 28u;

            uint32_t high = 0;
            high |= texID & 0x0FFFu;
            high |= (animFrames & 0x1Fu) << 12u;
            high |= (r >> 3 & 0x1Fu) << 17u;
            high |= (g >> 3 & 0x1Fu) << 22u;
            high |= (b >> 3 & 0x1Fu) << 27u;

            data = static_cast<uint64_t>(high) << 32u | low;
        }
    };

}

#endif //CUBE_VERTEX_HPP
