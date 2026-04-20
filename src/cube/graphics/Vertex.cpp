#include <cube/graphics/Vertex.hpp>

namespace cube {

    Vertex Vertex::pack(
        const uint8_t x, const uint8_t y, const uint8_t z,
        const uint8_t ao, const uint8_t u, const uint8_t v,
        const uint8_t norm, const uint8_t light, const bool sway,
        const uint8_t id
    ) {
        uint32_t p = 0;
        p |= x & 0x0F;                          // 0-3   (4 bits)   -> X
        p |= (y & 0x0F) << 4;                   // 4-7   (4 bits)   -> Y
        p |= (z & 0x0F) << 8;                   // 8-11  (4 bits)   -> Z
        p |= (ao & 0x03) << 12;                 // 12-13 (2 bits)   -> AO
        p |= (u  & 0x01) << 14;                 // 14    (1 bit)    -> U
        p |= (v  & 0x01) << 15;                 // 15    (1 bit)    -> V
        p |= (norm & 0x07) << 16;               // 16-18 (3 bits)   -> Normal
        p |= (light & 0x0F) << 19;              // 19-22 (4 bits)   -> Light
        p |= (sway ? 1 : 0) << 23;              // 23    (1 bit)    -> Sway
        p |= static_cast<uint32_t>(id) << 24;   // 24-31 (8 bits)   -> BlockID
        return { p };
    }

}