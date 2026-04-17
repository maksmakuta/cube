#ifndef CUBE_CAMERA_HPP
#define CUBE_CAMERA_HPP

#include <glm/glm.hpp>

namespace cube {

    class Camera final {
    public:
        explicit Camera(const glm::vec3& position = {0.0f, 0.0f, 0.0f});

        void rotate(float deltaX, float deltaY);
        void setPosition(const glm::vec3& newPos);

        [[nodiscard]] glm::vec3 getPosition() const;
        [[nodiscard]] glm::vec3 getForward() const;
        [[nodiscard]] glm::vec3 getRight() const;
        [[nodiscard]] glm::vec3 getUp() const;
        [[nodiscard]] glm::vec2 getDirection() const;
        [[nodiscard]] glm::mat4 getViewMatrix() const;

    private:
        void updateVectors();

        glm::vec3 m_position;
        glm::vec3 m_forward{0.0f, 0.0f, -1.0f};
        glm::vec3 m_up{0.0f, 1.0f, 0.0f};
        glm::vec3 m_right{1.0f, 0.0f, 0.0f};
        glm::vec3 m_world_up{0.0f, 1.0f, 0.0f};

        glm::vec2 rotationAngles{-90.0f, 0.0f};
        float sensitivity{0.1f};
    };
}

#endif //CUBE_CAMERA_HPP
