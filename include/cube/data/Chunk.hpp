#ifndef CUBE_CHUNK_HPP
#define CUBE_CHUNK_HPP

#include <vector>
#include <mdspan>
#include <glm/vec3.hpp>

#include "Block.hpp"

namespace cube {

    constexpr auto CHUNK_SIZE = glm::ivec3(16);
    constexpr auto CHUNK_LENGTH = CHUNK_SIZE.x * CHUNK_SIZE.y * CHUNK_SIZE.z;

    class Chunk final {
    public:
        Chunk() = default;

        auto view() {
            return std::mdspan<Block, std::extents<size_t, CHUNK_SIZE.x, CHUNK_SIZE.y, CHUNK_SIZE.z>>(m_blocks.data());
        }

        [[nodiscard]] auto view() const {
            return std::mdspan<const Block, std::extents<size_t, CHUNK_SIZE.x, CHUNK_SIZE.y, CHUNK_SIZE.z>>(m_blocks.data());
        }

    private:
        std::vector<Block> m_blocks{CHUNK_LENGTH, Air};
    };

    inline int dist(const glm::ivec3& a, const glm::ivec3& b) {
        const int dx = a.x - b.x;
        const int dy = a.y - b.y;
        const int dz = a.z - b.z;
        return dx * dx + dy * dy + dz * dz;
    }

}

#endif //CUBE_CHUNK_HPP
