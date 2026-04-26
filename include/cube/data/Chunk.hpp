#ifndef CUBE_CHUNK_HPP
#define CUBE_CHUNK_HPP

#include <array>
#include <mdspan>
#include <glm/vec3.hpp>

#include "Block.hpp"

namespace cube {

    constexpr auto CHUNK_SIZE = glm::ivec3(16);
    constexpr auto CHUNK_LENGTH = CHUNK_SIZE.x * CHUNK_SIZE.y * CHUNK_SIZE.z;

    enum class ChunkState {
        Unloaded,
        Generating,
        Generated,
        Meshing,
        Renderable,
    };

    struct NoiseValues {
        float temp;
        float moisture;
    };

    class Chunk final {
    public:
        Chunk() = default;

        auto view() {
            return std::mdspan<Block, std::extents<size_t, CHUNK_SIZE.x, CHUNK_SIZE.y, CHUNK_SIZE.z>>(m_blocks.data());
        }

        [[nodiscard]] auto view() const {
            return std::mdspan<const Block, std::extents<size_t, CHUNK_SIZE.x, CHUNK_SIZE.y, CHUNK_SIZE.z>>(m_blocks.data());
        }

        [[nodiscard]] ChunkState state() const {
            return m_state;
        }

        void setState(const ChunkState state) {
            m_state = state;
        }

        std::array<NoiseValues, 256> biomeData{};

    private:
        ChunkState m_state = ChunkState::Unloaded;
        std::array<Block, CHUNK_LENGTH> m_blocks{};
    };

}

#endif //CUBE_CHUNK_HPP
