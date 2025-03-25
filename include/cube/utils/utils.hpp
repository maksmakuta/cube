#ifndef UTILS_HPP
#define UTILS_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace cube::utils {

    glm::ivec2 toChunkPos(const glm::vec3 &pos);
    void setBit(uint8_t& value, uint8_t mask, bool action);

}

#endif //UTILS_HPP
