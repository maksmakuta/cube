#ifndef CUBE_CHUNK_HPP
#define CUBE_CHUNK_HPP

#include <array>
#include <mdspan>
#include <glm/vec3.hpp>

namespace cube {

    constexpr auto CHUNK_SIZE = glm::ivec3(16,16,16);
    constexpr auto CHUNK_FLAT_SIZE = CHUNK_SIZE.x * CHUNK_SIZE.y * CHUNK_SIZE.z;

    struct Chunk {
        std::array<uint8_t, CHUNK_FLAT_SIZE> data;

        [[nodiscard]] auto view() const {
            return std::mdspan<const uint8_t, std::extents<uint32_t, CHUNK_SIZE.x, CHUNK_SIZE.y,CHUNK_SIZE.z>>(data.data());
        }

        auto view() {
            return std::mdspan<uint8_t, std::extents<uint32_t, CHUNK_SIZE.x, CHUNK_SIZE.y,CHUNK_SIZE.z>>(data.data());
        }
    };

}

#endif //CUBE_CHUNK_HPP
