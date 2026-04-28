#ifndef CUBE_CAMERA_HPP
#define CUBE_CAMERA_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace cube {

    class Camera {
    public:
        explicit Camera(const glm::vec3& position = {0.0f, 0.0f, 0.0f});

        [[nodiscard]] glm::mat4 getViewMatrix() const;

        void rotate(const glm::vec2& offset);
        void moveFPS(float forward, float right, float up = 0.0f);
        void setPosition(const glm::vec3& position);

        [[nodiscard]] glm::vec3 getPosition() const;
        [[nodiscard]] glm::vec3 getFront() const;

    private:
        void updateCameraVectors();

        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_right;
        glm::vec3 m_up;
        glm::vec3 m_worldUp;
        glm::vec2 m_direction;
    };

}

#endif //CUBE_CAMERA_HPP
