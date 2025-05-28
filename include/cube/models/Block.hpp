#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <cstdint>
#include <glm/vec4.hpp>

#include "cube/enums/Face.hpp"

namespace cube {

    enum class BlockID : uint8_t{
        Air,
        Grass,
        Dirt,
        Sand,
        Stone,
        Graystone,
        WoodDark,
        WoodLight,
        LeavesGreen,
        LeavesOrange,
        MushroomRed,
        MushroomBrown,
        Cactus,
        WeedDry,
        Weed,
        Water
    };

    glm::vec4 getTile(BlockID, Face);
}

#endif //BLOCK_HPP
