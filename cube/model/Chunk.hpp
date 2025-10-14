#ifndef CUBE_CHUNK_HPP
#define CUBE_CHUNK_HPP

#include <vector>
#include <glm/vec3.hpp>

#include "Block.hpp"

namespace cube {

    constexpr auto CHUNK_WIDTH = 16;
    constexpr auto CHUNK_DEPTH = 16;
    constexpr auto CHUNK_HEIGHT = 128;
    constexpr auto CHUNK_SIZE = CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT;

    inline bool inChunk(const glm::ivec3& pos) {
        return  static_cast<unsigned>(pos.x) < CHUNK_WIDTH &&
                static_cast<unsigned>(pos.y) < CHUNK_HEIGHT &&
                static_cast<unsigned>(pos.z) < CHUNK_DEPTH;
    }

    inline int index3D(const glm::ivec3& pos) {
        return pos.x + pos.z * CHUNK_WIDTH + pos.y * CHUNK_WIDTH * CHUNK_DEPTH;
    }

    class Chunk {
    public:
        Chunk();
        ~Chunk();

        [[nodiscard]] BlockID get(const glm::ivec3& pos) const;
        void set(const glm::ivec3& pos, BlockID id);

    private:
        bool m_dirty = false;
        std::vector<BlockID> m_blocks;
    };

}

#endif //CUBE_CHUNK_HPP