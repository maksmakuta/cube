#ifndef CUBE_CAMERA_HPP
#define CUBE_CAMERA_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace cube {

    enum Movement {
        Forward     = 1 << 1,
        Backward    = 1 << 2,
        Left        = 1 << 3,
        Right       = 1 << 4,
        Up          = 1 << 5,
        Down        = 1 << 6,
    };

    class Camera {
    public:
        Camera();

        [[nodiscard]] glm::mat4 getViewMatrix() const;
        [[nodiscard]] float getFOV() const;

        void setFOV(float);

        void update(float dt);
        void move(uint8_t);
        void rotate(float, float);

        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 delta{0};
        float yaw;
        float pitch;
        float fov;
    };

}

#endif //CUBE_CAMERA_HPP