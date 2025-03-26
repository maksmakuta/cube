#include "cube/render/camera.hpp"

#include "cube/core/constants.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace cube::render {

    Camera::Camera(const glm::vec3& position,const glm::vec3& up,const glm::vec3& front ) : m_position(position),
        m_up(up), m_right(), m_rotation(), m_front(front) {
        rotate({});
    }

    void Camera::setPosition(const glm::vec3& position){
        m_position = position;
    }

    void Camera::setRotation(const glm::vec2& rotation){
        m_rotation = rotation;
    }

    glm::vec3 Camera::getPosition() const{
        return m_position;
    }

    glm::vec2 Camera::getRotation() const{
        return m_rotation;
    }

    glm::mat4 Camera::getMatrix() const{
        return lookAt(m_position,m_position + m_front, m_up);
    }

    void Camera::update(glm::uint8_t direction, float delta) {
        if (direction == 0) {
            return;
        }

        const auto velocity = CAMERA_SPEED * delta;

        if (direction & FORWARD) {
            m_position += m_front * velocity;
        }
        if (direction & BACKWARD) {
            m_position -= m_front * velocity;
        }
        if (direction & RIGHT) {
            m_position += m_right * velocity;
        }
        if (direction & LEFT) {
            m_position -= m_right * velocity;
        }
        if (direction & UP) {
            m_position += m_up * velocity;
        }
        if (direction & DOWN) {
            m_position -= m_up * velocity;
        }
    }

    void Camera::rotate(const glm::vec2& delta) {
        m_rotation += delta * CAMERA_SENSITIVITY;

        if (m_rotation.y > 89.0f)
            m_rotation.y = 89.0f;
        if (m_rotation.y < -89.0f)
            m_rotation.y = -89.0f;

        if (m_rotation.x > 360.0f) {
            m_rotation.x -= 360.0f;
        }
        if (m_rotation.x < 0.0f) {
            m_rotation.x += 360.0f;
        }

        glm::vec3 front;
        front.x = cosf(glm::radians(m_rotation.x)) * cosf(glm::radians(m_rotation.y));
        front.y = sinf(glm::radians(m_rotation.y));
        front.z = sinf(glm::radians(m_rotation.x)) * cosf(glm::radians(m_rotation.y));
        m_front = normalize(front);
        m_right = normalize(cross(m_front, m_up));
    }

}
