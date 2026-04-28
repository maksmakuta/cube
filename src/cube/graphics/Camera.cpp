#include "cube/graphics/Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace cube {

     Camera::Camera(const glm::vec3& position)
        : m_position(position),
          m_worldUp(0.0f, 1.0f, 0.0f),
          m_direction(-90.0f, 0.0f)
    {
        updateCameraVectors();
    }

    glm::mat4 Camera::getViewMatrix() const {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    void Camera::rotate(const glm::vec2& offset) {
        m_direction += offset;
        m_direction.y = std::clamp(m_direction.y, -89.0f, 89.0f);
        updateCameraVectors();
    }

    void Camera::moveFPS(const float forward, const float right, const float up) {
         const glm::vec3 groundFront = glm::normalize(glm::vec3(m_front.x, 0.0f, m_front.z));
         const glm::vec3 groundRight = glm::normalize(glm::cross(groundFront, m_worldUp));

         m_position += groundFront * forward;
         m_position += groundRight * right;
         m_position += m_worldUp * up;
    }

    void Camera::setPosition(const glm::vec3& position) { m_position = position; }

    glm::vec3 Camera::getPosition() const { return m_position; }
    glm::vec3 Camera::getFront() const { return m_front; }

    void Camera::updateCameraVectors() {
        glm::vec3 front;
        front.x = std::cos(glm::radians(m_direction.x)) * std::cos(glm::radians(m_direction.y));
        front.y = std::sin(glm::radians(m_direction.y));
        front.z = std::sin(glm::radians(m_direction.x)) * std::cos(glm::radians(m_direction.y));

        m_front = glm::normalize(front);
        m_right = glm::normalize(glm::cross(m_front, m_worldUp));
        m_up    = glm::normalize(glm::cross(m_right, m_front));
    }

}

