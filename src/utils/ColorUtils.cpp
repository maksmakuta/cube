#include "cube/utils/ColorUtils.hpp"

namespace cube {

    glm::vec4 toVec4(const Color& c) {
        return {
            static_cast<float>(c >> 16 & 0xFF) / 255.f,
            static_cast<float>(c >> 8 & 0xFF) / 255.f,
            static_cast<float>(c >> 0 & 0xFF) / 255.f,
            static_cast<float>(c >> 24 & 0xFF) / 255.f,
        };
    }

}
