#include "Chunk.hpp"

namespace cube {

    Chunk::Chunk() : m_blocks(CHUNK_SIZE, BLOCK_AIR){}
    Chunk::~Chunk() = default;

    BlockID Chunk::get(const glm::ivec3& pos) const {
        if (!inChunk(pos)) return BLOCK_AIR;
        return m_blocks[index3D(pos)];
    }

    void Chunk::set(const glm::ivec3& pos, const BlockID id) {
        if (!inChunk(pos)) return;
        m_blocks[index3D(pos)] = id;
        m_dirty = true;
    }

}