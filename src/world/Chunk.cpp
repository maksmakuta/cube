#include "cube/world/Chunk.hpp"

#include <iostream>

namespace cube {

    Chunk::Chunk() = default;

    Chunk::~Chunk() = default;

    int getIndex(const glm::ivec3& pos) {
        if ((pos.x >= 0 && pos.x < CHUNK_WIDTH) &&
            (pos.y >= 0 && pos.y < CHUNK_HEIGHT) &&
            (pos.z >= 0 && pos.z < CHUNK_DEPTH)) {
            return pos.x + CHUNK_WIDTH * (pos.z + CHUNK_DEPTH * pos.y);
        }
        return -1;
    }

    BlockID & Chunk::at(const glm::ivec3 &pos) {
        const auto i = getIndex(pos);
        if (i == -1) {
            std::cout << "Error: bad position [" << pos.x << ", "<< pos.y << ", "<< pos.z << "]" << std::endl;
        }
        return m_blocks[i];
    }

    bool Chunk::is(const glm::ivec3 &pos, const BlockID b) const {
        const auto i = getIndex(pos);
        if (i == -1 ) {
            return false;
        }
        return m_blocks[i] == b;
    }
}
