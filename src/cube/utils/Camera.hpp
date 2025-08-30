#ifndef CUBE_CAMERA_HPP
#define CUBE_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

    [[nodiscard]] glm::mat4 getViewMatrix() const;

    void processKeyboard(Camera_Movement direction, float deltaTime);

    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    [[nodiscard]] glm::vec3 getPosition() const { return position; }

private:
    void updateCameraVectors();

    glm::vec3 position;
    glm::vec3 front{};
    glm::vec3 up{};
    glm::vec3 right{};
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;
};

#endif //CUBE_CAMERA_HPP
