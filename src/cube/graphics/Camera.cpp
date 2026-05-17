#include "cube/graphics/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace cube {

    Camera::Camera(float fov_angle, const glm::vec3& position)
        : m_position(position)
        , m_front(glm::vec3(0.0f, 0.0f, -1.0f))
        , m_right(glm::vec3(1.0f, 0.0f, 0.0f))
        , m_up(glm::vec3(0.0f, 1.0f, 0.0f))
        , m_direction(glm::vec2(-90.0f, 0.0f))
        , m_size(glm::vec2(1280.0f, 720.0f))
        , m_fov(fov_angle) {
        update();
    }

    glm::mat4 Camera::getViewMatrix() const {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    glm::mat4 Camera::getProjection() const {
        const float aspect = m_size.y > 0.0f ? m_size.x / m_size.y : 16.0f / 9.0f;
        return glm::perspective(glm::radians(m_fov), aspect, 0.1f, 2048.0f);
    }

    glm::vec3 Camera::getPosition() const { return m_position; }
    glm::vec3 Camera::getFront() const    { return m_front; }
    glm::vec3 Camera::getRight() const    { return m_right; }
    glm::vec3 Camera::getUp() const       { return m_up; }
    glm::vec2 Camera::getDirection() const{ return m_direction; }

    void Camera::rotate(const glm::vec2& delta) {
        m_direction.x += delta.x;
        m_direction.y += delta.y;

        m_direction = glm::clamp(m_direction, -89.0f, 89.0f);

        update();
    }

    void Camera::resize(const glm::vec2& size) {
        m_size = size;
    }

    void Camera::moveTo(const glm::vec3& pos) {
        m_position = pos;
    }

    void Camera::update() {
        glm::dvec3 front;
        front.x = cos(glm::radians(m_direction.x)) * cos(glm::radians(m_direction.y));
        front.y = sin(glm::radians(m_direction.y));
        front.z = sin(glm::radians(m_direction.x)) * cos(glm::radians(m_direction.y));

        m_front = glm::normalize(front);

        constexpr glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        m_right = glm::normalize(glm::cross(m_front, worldUp));
        m_up    = glm::normalize(glm::cross(m_right, m_front));
    }

}