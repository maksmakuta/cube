#include "cube/world/Chunk.hpp"

namespace cube {

    Chunk::Chunk(const glm::ivec2& offset) : m_offset(offset){}
    Chunk::~Chunk() = default;

    glm::uint toIndex(const glm::uvec3& p) {
        return (p.z * CHUNK_HEIGHT * CHUNK_WIDTH) + (p.y * CHUNK_WIDTH) + p.x;
    }

    BlockID& Chunk::at(const glm::uint index) {
        assert(index < m_blocks.size());
        return m_blocks[index];
    }

    BlockID& Chunk::at(const glm::uvec3& pos) {
        return at(toIndex(pos));
    }

    BlockID Chunk::get(const glm::uvec3& pos)const {
        const auto index = toIndex(pos);
        if (index > CHUNK_SIZE) {
            return BlockID::Air;
        }
        return m_blocks[index];
    }

    bool Chunk::is(const glm::uvec3& pos, const BlockID i) const {
        return get(pos) == i;
    }

    glm::ivec2 Chunk::getOffset() const {
        return m_offset;
    }

}