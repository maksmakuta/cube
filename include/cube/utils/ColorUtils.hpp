#ifndef COLORUTILS_HPP
#define COLORUTILS_HPP

#include <glm/vec4.hpp>

namespace cube {

    /*
     * Color in hex format: 0xAARRGGBB
     */
    using Color = glm::uint;

    glm::vec4 toVec4(const Color&);

}
#endif //COLORUTILS_HPP
