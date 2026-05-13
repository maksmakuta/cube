#ifndef CUBE_VERTEX_HPP
#define CUBE_VERTEX_HPP

#include <cstdint>

namespace cube {

    struct Vertex final {
        uint32_t low;
        uint32_t high;

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
        ) {
            low = 0;
            low |= (static_cast<uint32_t>(x) & 0x1Fu);
            low |= (static_cast<uint32_t>(y) & 0x1Fu) << 5u;
            low |= (static_cast<uint32_t>(z) & 0x1Fu) << 10u;
            low |= (static_cast<uint32_t>(n) & 0x07u) << 15u;
            low |= (static_cast<uint32_t>(u) & 0x03u) << 18u;
            low |= (static_cast<uint32_t>(v) & 0x03u) << 20u;
            low |= (static_cast<uint32_t>(ao) & 0x03u) << 22u;
            low |= (static_cast<uint32_t>(overlay) & 0x0Fu) << 24u;
            low |= (static_cast<uint32_t>(animSpeed) & 0x07u) << 28u;

            high = 0;
            high |= (static_cast<uint32_t>(texID) & 0x0FFFu);
            high |= (static_cast<uint32_t>(animFrames) & 0x1Fu) << 12u;
            high |= (static_cast<uint32_t>(r >> 3) & 0x1Fu) << 17u;
            high |= (static_cast<uint32_t>(g >> 3) & 0x1Fu) << 22u;
            high |= (static_cast<uint32_t>(b >> 3) & 0x1Fu) << 27u;
        }
    };

}

#endif //CUBE_VERTEX_HPP
