#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <cstdint>
#include <vector>

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

    class Chunk {
    public:
        Chunk() = default;

        [[nodiscard]] BlockData at(uint8_t x, uint8_t y, uint8_t z) const;
        void set(uint8_t x, uint8_t y, uint8_t z, const BlockData &data);

    private:
        std::vector<BlockData> m_data;
    };

}

#endif //CHUNK_HPP
