#ifndef CUBE_VERTEX_HPP
#define CUBE_VERTEX_HPP

#include <cstdint>

namespace cube {

    // vertex can store multiple data in very tiny and optimized format
    using Vertex = uint64_t;

    /**
     * compress vertex with default data
     * @param x X pos
     * @param y Y pos
     * @param z Z pos
     * @param u U texcoord
     * @param v V texcoord
     * @param t texture layer
     * @param a ambient occlusion value
     * @return Compressed Vertex
     */
    Vertex compress(
        uint8_t x ,uint8_t y, uint8_t z,
        uint8_t u, uint8_t v, uint8_t t,
        uint8_t a
    );

}

#endif //CUBE_VERTEX_HPP
