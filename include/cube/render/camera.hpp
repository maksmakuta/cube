#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace cube::render {

    enum CameraDirection {
        FORWARD     = 1 << 1,
        BACKWARD    = 1 << 2,
        LEFT        = 1 << 3,
        RIGHT       = 1 << 4,
        DOWN        = 1 << 5,
        UP          = 1 << 6
    };

    class Camera {
    public:
        explicit Camera(
            const glm::vec3& position = {0,0,0},
            const glm::vec3& up = {0,1,0},
            const glm::vec3& front = {1,0,0}
        );

        void setPosition(const glm::vec3& position);
        void setRotation(const glm::vec2& rotation);

        [[nodiscard]] glm::vec3 getPosition() const;
        [[nodiscard]] glm::vec2 getRotation() const;
        [[nodiscard]] glm::mat4 getMatrix() const;

        void update(glm::uint8_t direction, float delta);
        void rotate(const glm::vec2& delta);
    private:
        glm::vec3 m_position;
    };

}

#endif //CAMERA_HPP
