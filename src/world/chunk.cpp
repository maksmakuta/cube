#include "cube/core/constants.hpp"
#include "cube/world/chunk.hpp"

namespace cube::world {

    BlockData Chunk::at(const uint8_t x, const uint8_t y, const uint8_t z) const {
        return m_data[z * CHUNK_HEIGHT * CHUNK_WIDTH + y * CHUNK_WIDTH + x];
    }

    void Chunk::set(const uint8_t x, const uint8_t y, const uint8_t z, const BlockData &data) {
        m_data[z * CHUNK_HEIGHT * CHUNK_WIDTH + y * CHUNK_WIDTH + x] = data;
    }

}
