#include "Chunk.hpp"

namespace cube {
    Chunk::Chunk() : m_blocks(CHUNK_SIZE.x * CHUNK_SIZE.y * CHUNK_SIZE.z, Block::Air) {}
    Chunk::~Chunk() = default;

    bool inside(const BlockPos& pos) {
        return static_cast<unsigned>(pos.x) < CHUNK_SIZE.x &&
                static_cast<unsigned>(pos.y) < CHUNK_SIZE.y &&
                static_cast<unsigned>(pos.z) < CHUNK_SIZE.z;
    }

    int toIndex(const BlockPos& pos) {
        return pos.x + pos.y * CHUNK_SIZE.x + pos.z * CHUNK_SIZE.x * CHUNK_SIZE.y;
    }

    void Chunk::setBlock(const BlockPos& pos, const Block b) {
        if (inside(pos)) {
            m_blocks[toIndex(pos)] = b;
        }
    }

    Block Chunk::getBlock(const BlockPos& pos) const {
        if (!inside(pos)) {
            return Block::Air;
        }
        return m_blocks[toIndex(pos)];
    }

}