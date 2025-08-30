#include "cube/utils/Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position(position), worldUp(up), yaw(yaw), pitch(pitch),
      movementSpeed(5.0f), mouseSensitivity(0.1f) {
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    if (direction == Camera_Movement::FORWARD)
        position += front * velocity;
    if (direction == Camera_Movement::BACKWARD)
        position -= front * velocity;
    if (direction == Camera_Movement::LEFT)
        position -= right * velocity;
    if (direction == Camera_Movement::RIGHT)
        position += right * velocity;
    if (direction == Camera_Movement::UP)
        position += worldUp * velocity;
    if (direction == Camera_Movement::DOWN)
        position -= worldUp * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch) {
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 f;
    f.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
    f.y = sinf(glm::radians(pitch));
    f.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
    front = glm::normalize(f);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
