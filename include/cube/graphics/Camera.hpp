#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/mat4x4.hpp>

namespace cube {

    class Camera {
    public:
        Camera();
        ~Camera();

    private:
        glm::mat4 m_view{1.f};
        glm::vec2 m_rotation{0};
    };

}

#endif //CAMERA_HPP
