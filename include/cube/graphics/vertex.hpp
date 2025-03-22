#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <cstdint>

namespace cube::graphics {

    /**
     *  bits: 32                                       0
     *          uuvv tttt zzzz zzzz yyyy yyyy xxxx xxxx
     *
     *  legend:
     *  x - X coordinate
     *  y - Y coordinate
     *  z - Z coordinate
     *  t - world light
     *  u - U texture coordinate
     *  v - V texture coordinate
     */
    using Vertex = uint32_t;

}

#endif //VERTEX_HPP
