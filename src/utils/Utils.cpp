#include "cube/utils/Utils.hpp"

#include "cube/core/Constants.hpp"

#ifndef CUBE_ASSETS_DIR
#error "CUBE_ASSETS_DIR is not defined, please define assets dir"
#endif

namespace cube {

    std::string getAsset(const std::string& path) {
        return CUBE_ASSETS_DIR + path;
    }

    glm::vec4 toVec4(const Color& c) {
        return {
            static_cast<float>(c >> 16 & 0xFF) / 255.f,
            static_cast<float>(c >> 8 & 0xFF) / 255.f,
            static_cast<float>(c >> 0 & 0xFF) / 255.f,
            static_cast<float>(c >> 24 & 0xFF) / 255.f,
        };
    }

    AABB getChunkAABB(const glm::ivec2& chunkPos) {
        const auto base = glm::vec3(chunkPos.x * CHUNK_WIDTH, 0, chunkPos.y * CHUNK_DEPTH);
        return {
            base,
            base + glm::vec3(CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH)
        };
    }

    Frustum extractFrustum(const glm::mat4& vp) {
        Frustum f{};
        // Left
        f.planes[0] = glm::vec4(
            vp[0][3] + vp[0][0],
            vp[1][3] + vp[1][0],
            vp[2][3] + vp[2][0],
            vp[3][3] + vp[3][0]
        );
        // Right
        f.planes[1] = glm::vec4(
            vp[0][3] - vp[0][0],
            vp[1][3] - vp[1][0],
            vp[2][3] - vp[2][0],
            vp[3][3] - vp[3][0]
        );
        // Bottom
        f.planes[2] = glm::vec4(
            vp[0][3] + vp[0][1],
            vp[1][3] + vp[1][1],
            vp[2][3] + vp[2][1],
            vp[3][3] + vp[3][1]
        );
        // Top
        f.planes[3] = glm::vec4(
            vp[0][3] - vp[0][1],
            vp[1][3] - vp[1][1],
            vp[2][3] - vp[2][1],
            vp[3][3] - vp[3][1]
        );
        // Near
        f.planes[4] = glm::vec4(
            vp[0][3] + vp[0][2],
            vp[1][3] + vp[1][2],
            vp[2][3] + vp[2][2],
            vp[3][3] + vp[3][2]
        );
        // Far
        f.planes[5] = glm::vec4(
            vp[0][3] - vp[0][2],
            vp[1][3] - vp[1][2],
            vp[2][3] - vp[2][2],
            vp[3][3] - vp[3][2]
        );
        return f;
    }

    bool isAABBVisible(const AABB& box, const Frustum& f) {
        for (const auto& plane : f.planes) {
            glm::vec3 positiveVertex = box.min;
            if (plane.x >= 0) positiveVertex.x = box.max.x;
            if (plane.y >= 0) positiveVertex.y = box.max.y;
            if (plane.z >= 0) positiveVertex.z = box.max.z;

            const float distance = plane.x * positiveVertex.x + plane.y * positiveVertex.y + plane.z * positiveVertex.z + plane.w;
            if (distance < 0)
                return false;
        }
        return true;
    }

    glm::vec2 toChunk(const glm::vec3 &p) {
        return glm::floor(glm::vec2{p.x,p.z} / glm::vec2{CHUNK_WIDTH, CHUNK_DEPTH});
    }
}
