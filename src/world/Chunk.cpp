#include "cube/world/Chunk.hpp"

namespace cube {

    Chunk::Chunk() : m_blocks(CHUNK_SIZE, BlockID::Air) {}
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

    void Chunk::set(const glm::uvec3 &pos, const BlockID id) {
        m_blocks[toIndex(pos)] = id;
    }

    BlockID Chunk::get(const glm::uvec3& pos)const {
        const auto index = toIndex(pos);
        if (index > CHUNK_SIZE) {
            return BlockID::Air;
        }
        return m_blocks[index];
    }

    bool Chunk::is(const glm::ivec3& pos, const BlockID i) const {
        if (pos.x < 0 || pos.x >= CHUNK_WIDTH ||
            pos.y < 0 || pos.y >= CHUNK_HEIGHT ||
            pos.z < 0 || pos.z >= CHUNK_DEPTH )
            return false;
        return get(pos) == i;
    }

}