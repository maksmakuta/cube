#ifndef FRUSTUMCULLING_HPP
#define FRUSTUMCULLING_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

namespace cube {

    struct Frustum {
        glm::vec4 planes[6]{};

        Frustum();
        explicit Frustum(const glm::mat4& vp);

        bool isChunkVisible(const glm::vec3& min, const glm::vec3& max);
    };

}

#endif //FRUSTUMCULLING_HPP
