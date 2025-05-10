#include "cube/graphics/Camera.hpp"

#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "cube/core/Constants.hpp"

namespace cube {

    Camera::Camera(const glm::vec3 &pos) : m_position(pos){
        update();
    }

    Camera::~Camera() = default;

    void Camera::setPosition(const glm::vec3& p) {
       m_position = p;
    }

    void Camera::setRotation(const glm::vec2& r) {
        m_rotation = r;
        update();
    }

    void Camera::move(const uint8_t dir, const float dt) {
        if (dir == 0)
            return;

        if (dir & Forward) {
            m_position += m_front * (CAMERA_SPEED * dt);
        }
        if (dir & Backward) {
            m_position -= m_front * (CAMERA_SPEED * dt);
        }
        if (dir & Left) {
            m_position -= m_right * (CAMERA_SPEED * dt);
        }
        if (dir & Right) {
            m_position += m_right * (CAMERA_SPEED * dt);
        }
        if (dir & Up) {
            m_position += m_up * (CAMERA_SPEED * dt);
        }
        if (dir & Down) {
            m_position -= m_up * (CAMERA_SPEED * dt);
        }
    }

    void Camera::rotate(float dx, float dy) {
        m_rotation += glm::vec2{dx,dy} * CAMERA_SENSITIVITY;
        m_rotation.y = glm::clamp(m_rotation.y , -89.0f, 89.0f);
        update();
    }

    glm::vec3 Camera::getPosition() const {
        return m_position;
    }

    glm::vec2 Camera::getRotation() const {
        return m_rotation;
    }

    glm::mat4 Camera::getView() const {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    void Camera::update() {
        glm::vec3 f;
        f.x = cosf(glm::radians(m_rotation.x)) * cosf(glm::radians(m_rotation.y));
        f.y = sinf(glm::radians(m_rotation.y));
        f.z = sinf(glm::radians(m_rotation.x)) * cosf(glm::radians(m_rotation.y));
        m_front = glm::normalize(f);
        m_right = glm::normalize(glm::cross(m_front, CAMERA_WORLD_UP));
        m_up    = glm::normalize(glm::cross(m_right, m_front));
    }
}
