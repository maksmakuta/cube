#ifndef TYPES_HPP
#define TYPES_HPP

#include <cstdint>

namespace cube {


    /** bit  15                  0
     *        0000 0000 0000 0000
     *              cbb aaaa aaaa
     *
     *  legend:
     *  a - block id (256 variants - 8bits)
     *  b - direction of block (4 variants - 2 bits)
     *  c - upside down bit
     */
    using BlockData = uint16_t;

    using Seed = uint64_t;

}

#endif //TYPES_HPP
