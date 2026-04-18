#ifndef CUBE_CHUNK_HPP
#define CUBE_CHUNK_HPP

#include <array>
#include <mdspan>

#include "Blocks.hpp"

namespace cube {

    static constexpr int CHUNK_SIZE = 8;
    using BlockArray = std::array<Block, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE>;

    class Chunk final {
    public:
        Chunk() {
            m_blocks.fill(Air); // Ensure EVERY block is Air initially
        }

        [[nodiscard]] auto getView() const {
            return std::mdspan<const Block, std::extents<size_t, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE>>(m_blocks.data());
        }

        auto getMutableView() {
            return std::mdspan<Block, std::extents<size_t, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE>>(m_blocks.data());
        }

    private:
        alignas(64) BlockArray m_blocks{};
    };

}

#endif //CUBE_CHUNK_HPP
