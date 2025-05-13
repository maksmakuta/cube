#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <glm/glm.hpp>

namespace cube {

    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };

    template<class... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;

    template<class T, class S>
    void set(T& val, S bit, bool op) {
        op ? val |= bit : val &= ~bit;
    }

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
