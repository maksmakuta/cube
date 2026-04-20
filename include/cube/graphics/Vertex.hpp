#ifndef CUBE_VERTEX_HPP
#define CUBE_VERTEX_HPP

#include <cstdint>

namespace cube {

    struct Vertex final {
        uint32_t data;

        static Vertex pack(
            uint8_t x, uint8_t y, uint8_t z,
            uint8_t ao, uint8_t u, uint8_t v,
            uint8_t norm, uint8_t light, bool sway,
            uint8_t id
        );
    };

}

#endif //CUBE_VERTEX_HPP
