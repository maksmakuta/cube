#ifndef CUBE_CHUNK_HPP
#define CUBE_CHUNK_HPP

#include <vector>

#include "cube/core/Block.hpp"

#include <glm/vec3.hpp>

namespace cube {

    constexpr auto CHUNK_SIZE = 16;
    constexpr auto CHUNK_LEN = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

    class Chunk final {
    public:
        explicit Chunk(Block fill_block = Block::Air);

        Block& at(const glm::ivec3&);
        [[nodiscard]] Block at(const glm::ivec3&) const;

    private:
        std::vector<Block> m_blocks;
    };

}

#endif //CUBE_CHUNK_HPP
