#include "cube/graphics/FrustumCulling.hpp"

namespace cube {

    Frustum::Frustum() = default;

    Frustum::Frustum(const glm::mat4& vp) {
        const glm::vec4 rowX = vp[0];
        const glm::vec4 rowY = vp[1];
        const glm::vec4 rowZ = vp[2];
        const glm::vec4 rowW = vp[3];

        planes[0] = rowW + rowX;
        planes[1] = rowW - rowX;
        planes[2] = rowW + rowY;
        planes[3] = rowW - rowY;
        planes[4] = rowW + rowZ;
        planes[5] = rowW - rowZ;

        for (auto& p : planes) {
            const float len = glm::length(glm::vec3(p));
            p /= len;
        }
    }

    bool Frustum::isChunkVisible(const glm::vec3& min, const glm::vec3& max) {
        for (const auto& plane : planes) {
            auto normal = glm::vec3(plane);
            auto p = glm::vec3(
                (normal.x >= 0 ? max.x : min.x),
                (normal.y >= 0 ? max.y : min.y),
                (normal.z >= 0 ? max.z : min.z)
            );
            if (glm::dot(normal, p) + plane.w < 0)
                return false;
        }
        return true;
    }

}