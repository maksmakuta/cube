#ifndef UTILS_HPP
#define UTILS_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace cube::utils {

    glm::ivec2 toChunkPos(const glm::vec3 &pos);

}

#endif //UTILS_HPP
