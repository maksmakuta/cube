#include "cube/world/Chunk.hpp"

namespace cube {

    Chunk::Chunk() {
        m_blocks.reserve(CHUNK_SIZE);
    }

    Chunk::~Chunk() = default;

    int getIndex(const glm::ivec3& pos) {
        if (pos.x >= 0 && pos.x < CHUNK_WIDTH &&
            pos.y >= 0 && pos.y < CHUNK_HEIGHT &&
            pos.z >= 0 && pos.z < CHUNK_DEPTH) {
            return pos.x + CHUNK_WIDTH * (pos.z + CHUNK_DEPTH * pos.y);
        }
        return -1;
    }

    BlockID Chunk::get(const glm::ivec3& pos) const {
        return m_blocks[getIndex(pos)];
    }

    void Chunk::set(const glm::ivec3& pos,BlockID b) {
        m_blocks[getIndex(pos)] = b;
    }


}