#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "cube/core/block.hpp"
#include "cube/core/constants.hpp"
#include "cube/core/types.hpp"

#include <cstdint>
#include <vector>

namespace cube::world {

    class Chunk {
    public:
        Chunk() = default;

        [[nodiscard]] BlockData at(uint8_t x, uint8_t y, uint8_t z) const;
        void set(uint8_t x, uint8_t y, uint8_t z, const BlockData &data);

    private:
        std::vector<BlockData> m_data{CHUNK_SIZE, static_cast<BlockData>(BlockID::Air)};
    };

}

#endif //CHUNK_HPP
