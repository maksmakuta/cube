#include "cube/data/Chunk.hpp"

#include <format>
#include <stdexcept>

namespace cube {

    Chunk::Chunk() : voxels() {
        std::ranges::fill(voxels, 0);
    }

    bool inChunk(const glm::ivec3& pos) {
        return  pos.x >= 0 && pos.x < CHUNK_SIZE &&
                pos.y >= 0 && pos.y < CHUNK_SIZE &&
                pos.z >= 0 && pos.z < CHUNK_SIZE;
    }

    std::size_t indexOf(const glm::ivec3& pos) {
        return pos.x + pos.y * CHUNK_SIZE + pos.z * CHUNK_SIZE * CHUNK_SIZE;
    }

    Voxel& Chunk::at(const glm::ivec3& pos) {
        if (inChunk(pos)) {
            return voxels[indexOf(pos)];
        }
        throw std::out_of_range(std::format("Cannot access voxel at [{},{},{}]", pos.x,pos.y,pos.z));
    }

    [[nodiscard]] Voxel Chunk::at(const glm::ivec3& pos) const {
        if (inChunk(pos)) {
            return voxels[indexOf(pos)];
        }
        throw std::out_of_range(std::format("Cannot access voxel at [{},{},{}]", pos.x,pos.y,pos.z));
    }

}
