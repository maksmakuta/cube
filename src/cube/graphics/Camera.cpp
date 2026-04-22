#include <cube/graphics/Camera.hpp>

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace cube {

    Camera::Camera(const glm::vec3& pos)
        : m_position(pos)
        , m_front(0.0f, 0.0f, -1.0f)
        , m_right(1.0f, 0.0f, 0.0f)
        , m_direction(-90.0f, 0.0f)
    {
        updateVectors();
    }

    glm::mat4 Camera::getViewMatrix() const {
        return glm::lookAt(m_position, m_position + m_front, WORLD_UP);
    }

    glm::vec3 Camera::getPosition() const { return m_position; }
    glm::vec3 Camera::getFront() const { return m_front; }
    glm::vec3 Camera::getRight() const { return m_right; }
    glm::vec2 Camera::getDirection() const { return m_direction; }

    void Camera::move(const glm::vec3& position) {
        m_position = position;
    }

    void Camera::rotate(const glm::vec2& rotation) {
        m_direction += rotation;
        m_direction.y = std::clamp(m_direction.y, -89.0f, 89.0f);

        updateVectors();
    }

    void Camera::updateVectors() {
        glm::dvec3 front;
        front.x = cos(glm::radians(m_direction.x)) * cos(glm::radians(m_direction.y));
        front.y = sin(glm::radians(m_direction.y));
        front.z = sin(glm::radians(m_direction.x)) * cos(glm::radians(m_direction.y));

        m_front = glm::normalize(front);
        m_right = glm::normalize(glm::cross(m_front, WORLD_UP));
    }


}