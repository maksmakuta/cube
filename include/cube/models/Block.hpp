#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <glm/vec4.hpp>

namespace cube {

    enum class BlockID {
        Air,
        Grass,
        Dirt,
        Stone,
        Wood,
        Leaves,
        Bush,
        Flower,
        HardStone
    };

    glm::vec4 getTile(BlockID, int face);
    bool isFullBlock(BlockID);

}

#endif //BLOCK_HPP
