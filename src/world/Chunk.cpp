#include "cube/world/Chunk.hpp"

#include <glm/common.hpp>

#include "cube/core/Constants.hpp"

namespace cube {

    Chunk::Chunk() : m_data(CHUNK_SIZE, BlockID::Air){}
    Chunk::~Chunk() = default;

    inline int index(const int x, const int y, const int z){
        return x + CHUNK_WIDTH * (z + CHUNK_DEPTH * y);
    }

    void Chunk::setBlock(const int x, const int y, const int z, const BlockID b) {
        if (const auto i = index(x,y,z); i >= 0 && i < CHUNK_SIZE) {
            m_data[i] = b;
        }
    }

    void Chunk::setBlock(const glm::ivec3 &pos, const BlockID b) {
        setBlock(pos.x,pos.y,pos.z,b);
    }

    BlockID Chunk::getBlock(const int x, const int y, const int z) const {
        if (const auto i = index(x,y,z); i >= 0 && i < CHUNK_SIZE) {
            return m_data[i];
        }
        return BlockID::Air;
    }

    BlockID Chunk::getBlock(const glm::ivec3 &pos) const {
        return getBlock(pos.x,pos.y,pos.z);
    }

    glm::ivec3 clampToChunk(const glm::ivec3& v) {
        return glm::clamp(
            v,
            glm::ivec3(0),
            glm::ivec3(CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH) - 1
        );
    }

    void Chunk::fill(const glm::ivec3 &a, const glm::ivec3 &b, const BlockID blk) {
        if (a == b) {
            setBlock(a, blk);
            return;
        }

        const glm::ivec3 min = clampToChunk(glm::min(a, b));
        const glm::ivec3 max = clampToChunk(glm::max(a, b));

        for (int z = min.z; z <= max.z; ++z)
            for (int y = min.y; y <= max.y; ++y)
                for (int x = min.x; x <= max.x; ++x) {
                    setBlock(x, y, z, blk);
                }
    }

}
