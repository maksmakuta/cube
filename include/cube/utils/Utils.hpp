#ifndef UTILS_HPP
#define UTILS_HPP

#include <glm/glm.hpp>
#include <iostream>

#include "cube/core/Constants.hpp"

namespace cube {

    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };

    template<class... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;

    /*
     * Color in hex format: 0xAARRGGBB
     */
    using Color = glm::uint;

    struct AABB {
        glm::vec3 min;
        glm::vec3 max;
    };

    struct Frustum {
        glm::vec4 planes[6]; // left, right, bottom, top, near, far
    };

    std::string getAsset(const std::string& path);
    glm::vec4 toVec4(const Color&);
    AABB getChunkAABB(const glm::ivec2& chunkPos);
    Frustum extractFrustum(const glm::mat4& vp);
    bool isAABBVisible(const AABB& box, const Frustum& f);

}

#endif //UTILS_HPP
