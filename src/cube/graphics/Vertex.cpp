#include "cube/graphics/Vertex.hpp"

namespace cube {

    Vertex compress(
        const uint8_t x,
        const uint8_t y,
        const uint8_t z,
        const uint8_t u,
        const uint8_t v,
        const uint8_t t,
        const uint8_t a,
        const uint16_t tint
    ) {
        uint32_t lowBits = 0;
        lowBits |=  static_cast<uint32_t>(x) & 0x1F;
        lowBits |= (static_cast<uint32_t>(y) & 0x1F) << 5;
        lowBits |= (static_cast<uint32_t>(z) & 0x1F) << 10;
        lowBits |= (static_cast<uint32_t>(u) & 0x1F) << 15;
        lowBits |= (static_cast<uint32_t>(v) & 0x1F) << 20;

        uint32_t highBits = 0;
        highBits |=  static_cast<uint32_t>(t) & 0xFF;
        highBits |= (static_cast<uint32_t>(a) & 0x03) << 8;
        highBits |= static_cast<uint32_t>(tint) << 10;

        return static_cast<uint64_t>(highBits) << 32 | lowBits;
    }

}