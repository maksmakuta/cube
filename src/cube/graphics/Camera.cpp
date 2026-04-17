#include <cube/graphics/Camera.hpp>

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace cube {

    Camera::Camera(const glm::vec3& position) : m_position(position) {
        updateVectors();
    }

    [[nodiscard]] glm::mat4 Camera::getViewMatrix() const {
        return glm::lookAt(m_position, m_position + m_forward, m_up);
    }

    void Camera::rotate(float deltaX, float deltaY) {
        deltaX *= sensitivity;
        deltaY *= sensitivity;

        rotationAngles.x += deltaX;
        rotationAngles.y += deltaY;

        rotationAngles.y = std::clamp(rotationAngles.y, -89.0f, 89.0f);

        updateVectors();
    }

    void Camera::updateVectors() {
        glm::vec3 newForward;
        newForward.x = cosf(glm::radians(rotationAngles.x)) * cosf(glm::radians(rotationAngles.y));
        newForward.y = sinf(glm::radians(rotationAngles.y));
        newForward.z = sinf(glm::radians(rotationAngles.x)) * cosf(glm::radians(rotationAngles.y));

        m_forward = glm::normalize(newForward);

        m_right = glm::normalize(glm::cross(m_forward, m_world_up));
        m_up = glm::normalize(glm::cross(m_right, m_forward));
    }


    void Camera::setPosition(const glm::vec3& newPos) {
        m_position = newPos;
    }

    glm::vec3 Camera::getPosition() const {
        return m_position;
    }

    glm::vec3 Camera::getForward() const {
        return m_forward;
    }

    glm::vec3 Camera::getRight() const {
        return m_right;
    }

    glm::vec3 Camera::getUp() const {
        return m_world_up;
    }

    glm::vec2 Camera::getDirection() const {
        return rotationAngles;
    }

}