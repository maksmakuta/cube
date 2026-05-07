#include "cube/data/Chunk.hpp"

#include <format>
#include <stdexcept>

namespace cube {

    Chunk::Chunk(const Block fill_block) : m_blocks(CHUNK_LEN, fill_block) {}

    std::size_t indexOf(const glm::ivec3& pos) {
        if (pos.x >= 0 && pos.x < CHUNK_SIZE &&
            pos.y >= 0 && pos.y < CHUNK_SIZE &&
            pos.z >= 0 && pos.z < CHUNK_SIZE) {
            return pos.x + pos.y * CHUNK_SIZE + pos.z * CHUNK_SIZE * CHUNK_SIZE;
        }
        throw std::out_of_range(std::format("Chunk::indexOf({}, {}, {})", pos.x, pos.y, pos.z).c_str());
    }

    Block& Chunk::at(const glm::ivec3& pos) {
        return m_blocks[indexOf(pos)];
    }

    Block Chunk::at(const glm::ivec3& pos) const {
        return m_blocks[indexOf(pos)];
    }

}
