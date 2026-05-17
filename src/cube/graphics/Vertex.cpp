#include "cube/graphics/Vertex.hpp"

namespace cube {

    Vertex compress(
        const uint8_t x,
        const uint8_t y,
        const uint8_t z,
        const uint8_t u,
        const uint8_t v,
        const uint8_t t
    ) {
        Vertex compressed = 0;

        compressed |=  static_cast<Vertex>(x) & 0x1F;
        compressed |= (static_cast<Vertex>(y) & 0x1F) << 5;
        compressed |= (static_cast<Vertex>(z) & 0x1F) << 10;
        compressed |= (static_cast<Vertex>(u) & 0x1F) << 15;
        compressed |= (static_cast<Vertex>(v) & 0x1F) << 20;
        compressed |= (static_cast<Vertex>(t) & 0x7F) << 25;

        return compressed;
    }

}