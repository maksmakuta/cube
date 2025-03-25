#include "cube/core/constants.hpp"
#include "cube/utils/utils.hpp"

#include <glm/common.hpp>

namespace cube::utils {

    glm::ivec2 toChunkPos(const glm::vec3 &pos) {
        return glm::floor(glm::vec2(pos.x, pos.z) / glm::vec2(CHUNK_WIDTH, CHUNK_DEPTH));
    }

    void setBit(uint8_t &value, uint8_t mask, bool action) {
        if (action) {
            value |= mask;
        }else {
            value &= ~mask;
        }
    }
}
