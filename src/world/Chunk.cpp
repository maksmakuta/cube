#include "cube/world/Chunk.hpp"

namespace cube {

    Chunk::Chunk(const glm::vec2& offset) : m_offset(offset){}
    Chunk::~Chunk() = default;

    glm::uint toIndex(const glm::uvec3& p) {
        return p.x + CHUNK_WIDTH * (p.y + p.z * CHUNK_HEIGHT);
    }

    BlockID& Chunk::at(const glm::uint index) {
        assert(index < m_blocks.size());
        return m_blocks[index];
    }

    BlockID& Chunk::at(const glm::uvec3& pos) {
        return at(toIndex(pos));
    }

    BlockID Chunk::get(const glm::uvec3& pos)const {
        return m_blocks[toIndex(pos)];
    }

    bool Chunk::is(const glm::uvec3& pos, const BlockID i) const {
        return get(pos) == i;
    }

    glm::vec2 Chunk::getOffset() const {
        return m_offset;
    }

}