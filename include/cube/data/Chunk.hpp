#ifndef CUBE_CHUNK_HPP
#define CUBE_CHUNK_HPP

#include <vector>
#include <mdspan>

#include <glm/vec3.hpp>

#include "cube/core/Block.hpp"

namespace cube {

    constexpr auto CHUNK_SIZE = glm::ivec3(16);
    constexpr auto CHUNK_LENGTH = CHUNK_SIZE.x * CHUNK_SIZE.y * CHUNK_SIZE.z;

    using ChunkView = std::mdspan<
        Block,
        std::extents<size_t, CHUNK_SIZE.x, CHUNK_SIZE.y, CHUNK_SIZE.z>
    >;

    using ConstChunkView = std::mdspan<
        const Block,
        std::extents<size_t, CHUNK_SIZE.x, CHUNK_SIZE.y, CHUNK_SIZE.z>
    >;

    class Chunk final {
    public:
        explicit Chunk(const Block& block = Block::Air);

        ChunkView view();
        [[nodiscard]] ConstChunkView view() const;

        Block& operator[](size_t x, size_t y, size_t z);
        Block& operator[](const glm::ivec3& pos);
        Block operator[](size_t x, size_t y, size_t z) const;
        Block operator[](const glm::ivec3& pos) const;

    private:
        std::vector<Block> m_blocks;
    };
}

#endif //CUBE_CHUNK_HPP
