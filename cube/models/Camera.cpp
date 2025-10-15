#include "Camera.hpp"

#include <glm/ext/matrix_transform.hpp>

#include "utils/LambdaVisitor.hpp"

namespace cube {

    constexpr float CAMERA_SENSITIVITY = 0.005f;
    constexpr float CAMERA_SPEED = 5.f;

    void Camera::update(const float dt) {
        m_position += m_velocity * dt;
    }

    void Camera::onEvent(const Event& e) {
        std::visit(LambdaVisitor{
           [this](const KeyEvent& ke) {
                const auto right = glm::normalize(glm::cross(m_front, m_up));

               if (ke.pressed) {
                   if (ke.key == Key::W) {
                       m_velocity += m_front * CAMERA_SPEED;
                   }
                   if (ke.key == Key::S) {
                       m_velocity -= m_front * CAMERA_SPEED;
                   }
                   if (ke.key == Key::A) {
                       m_velocity -= right * CAMERA_SPEED;
                   }
                   if (ke.key == Key::D) {
                       m_velocity += right * CAMERA_SPEED;
                   }
                   if (ke.key == Key::Shift) {
                       m_velocity -= m_up * CAMERA_SPEED;
                   }
                   if (ke.key == Key::Space) {
                       m_velocity += m_up * CAMERA_SPEED;
                   }
               }else {
                   m_velocity = glm::vec3{0.f};
               }
           },
           [this](const cube::MouseEvent& me) {
               const auto a = glm::vec2{ me.x, me.y };
               m_rotation = glm::vec2{m_move.x - a.x, a.y - m_move.y} * CAMERA_SENSITIVITY;
               updateDirection();
               m_move = a;
           },
            [](const ResizeEvent&) {},
           [](const ScrollEvent&) {},
           [](const InputEvent&) {}
        }, e);
    }

    glm::mat4 Camera::getMatrix() const {
        return glm::lookAt(m_position,m_position + m_front, m_up);
    }

    glm::vec3 Camera::getPosition() const {
        return m_position;
    }

    glm::vec2 Camera::getRotation() const {
        return m_rotation;
    }

    float Camera::getFOV() const {
        return m_fov;
    }

    void Camera::setPosition(const glm::vec3& v) {
        m_position = v;
    }

    void Camera::setRotation(const glm::vec2& v) {
        m_rotation = v;
    }

    void Camera::setFOV(const float f) {
        m_fov = f;
    }

    void Camera::updateDirection() {
        const float yaw = m_rotation.x;
        const float pitch = m_rotation.y;

        m_front = glm::normalize(glm::vec3{
            cos(pitch) * cos(yaw),
            sin(pitch),
            cos(pitch) * sin(yaw)
        });
    }
}
