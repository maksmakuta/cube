#include "Camera.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace cube {

    constexpr auto sensitivity = 0.05f;
    constexpr auto speed = 5.f;

    Camera::Camera() :
        position(0,0,0), front(0,0,-1), up(0,1,0),
        yaw(-90.f), pitch(0.f), fov(70.f) {}

    glm::mat4 Camera::getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    float Camera::getFOV() const {
        return fov;
    }

    void Camera::setFOV(const float f) {
        fov = f;
    }

    void Camera::update(const float dt) {
        position += delta * dt;
    }

    void Camera::move(const uint8_t code) {
        if (code == 0) {
            delta = glm::vec3(0);
            return;
        }
        const glm::vec3 right = glm::normalize(glm::cross(front, up));

        if (code & Forward) {
            delta += front * speed;
        }
        if (code & Backward) {
            delta -= front * speed;
        }
        if (code & Left) {
            delta -= right * speed;
        }
        if (code & Right) {
            delta += right * speed;
        }
        if (code & Up) {
            delta += up * speed;
        }
        if (code & Down) {
            delta -= up * speed;
        }
    }

    void Camera::rotate(const float deltaX, const float deltaY) {
        yaw   += deltaX * sensitivity;
        pitch -= deltaY * sensitivity;
        pitch = glm::clamp(pitch, -89.0f, 89.0f);
        yaw = glm::mod(glm::abs(yaw), 360.0f);

        glm::dvec3 dir;
        dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        dir.y = sin(glm::radians(pitch));
        dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(dir);
    }

}
