#include "cube/graphics/Camera.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace cube {

    Camera::Camera(const glm::vec3& position) :
        m_position(position),
        m_front(0),
        m_right(0),
        m_direction(0),
        m_fov(70.f)
    {
        update();
    }

    glm::mat4 Camera::getViewMatrix() const {
        return glm::lookAt(
            m_position,
            m_position + m_front,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
    }

    glm::mat4 Camera::getProjMatrix(const glm::vec2& view, const float render_dist) const {
        return glm::perspective(
            glm::radians(m_fov),
            view.x / view.y,
            0.1f,
            render_dist
        );
    }

    glm::vec3 Camera::getPosition() const {
        return m_position;
    }

    glm::vec2 Camera::getDirection() const {
        return m_direction;
    }

    float Camera::getFov() const {
        return m_fov;
    }

    void Camera::setPosition(const glm::vec3& position) {
        m_position = position;
    }

    void Camera::setDirection(const glm::vec2& direction) {
        m_direction = direction;
        m_direction.y = glm::clamp(m_direction.y, -89.9f, 89.9f);
        update();
    }

    void Camera::setFov(const float fov) {
        m_fov = fov;
    }

    void Camera::update() {
        const float y = glm::radians(m_direction.x), p = glm::radians(m_direction.y);
        m_front = glm::normalize(glm::vec3{glm::cos(y) * glm::cos(p), glm::sin(p), glm::sin(y) * glm::cos(p)});
        m_right = glm::normalize(glm::cross(m_front, {0, 1, 0}));
    }

}
