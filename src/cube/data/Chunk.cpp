#include "cube/data/Chunk.hpp"

namespace cube {

    Chunk::Chunk(const Block& block) : m_blocks(CHUNK_LENGTH, block) {}

    ChunkView Chunk::view() {
        return std::mdspan<Block, std::extents<size_t, CHUNK_SIZE.x, CHUNK_SIZE.y, CHUNK_SIZE.z>>(m_blocks.data());
    }

    [[nodiscard]] ConstChunkView Chunk::view() const {
        return std::mdspan<const Block, std::extents<size_t, CHUNK_SIZE.x, CHUNK_SIZE.y, CHUNK_SIZE.z>>(m_blocks.data());
    }

    Block& Chunk::operator[](const size_t x, const size_t y, const size_t z) {
        return view()[x,y,z];
    }

    Block& Chunk::operator[](const glm::ivec3& pos) {
        return view()[pos.x,pos.y,pos.z];
    }

    Block Chunk::operator[](const size_t x, const size_t y, const size_t z) const {
        return view()[x,y,z];
    }

    Block Chunk::operator[](const glm::ivec3& pos) const {
        return view()[pos.x,pos.y,pos.z];
    }

}