#include "Camera.hpp"

#include <algorithm>
#include <glm/ext/matrix_transform.hpp>

#include "utils/LambdaVisitor.hpp"

namespace cube {

    constexpr float CAMERA_SENSITIVITY = 0.1f;
    constexpr float CAMERA_SPEED = 5.f;

    void Camera::update(const float dt) {
        const auto right = glm::normalize(glm::cross(m_front, m_up));
        glm::vec3 velocity{0.f};

        if(m_keyState[Key::W]) velocity += m_front;
        if(m_keyState[Key::S]) velocity -= m_front;
        if(m_keyState[Key::A]) velocity -= right;
        if(m_keyState[Key::D]) velocity += right;
        if(m_keyState[Key::Space]) velocity += m_up;
        if(m_keyState[Key::Shift]) velocity -= m_up;

        if(glm::length(velocity) > 0.f)
            velocity = glm::normalize(velocity) * CAMERA_SPEED;
        m_position += velocity * dt;
    }

    void Camera::onEvent(const Event& e) {
        std::visit(LambdaVisitor{
           [this](const KeyEvent& ke) {
               m_keyState[ke.key] = ke.pressed;
           },
           [this](const MouseEvent& me) {
               const auto a = glm::vec2{ me.x, me.y };
               m_rotation += glm::vec2{a.x - m_move.x, m_move.y - a.y} * CAMERA_SENSITIVITY;
               m_rotation.y = std::clamp(m_rotation.y, -89.f, 89.f);
               m_rotation.x = std::fmod(m_rotation.x + 360.0f, 360.0f);
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
        const float yaw = glm::radians(m_rotation.x);
        const float pitch = glm::radians(m_rotation.y);

        m_front = glm::normalize(glm::vec3{
            cos(pitch) * cos(yaw),
            sin(pitch),
            cos(pitch) * sin(yaw)
        });
    }
}
