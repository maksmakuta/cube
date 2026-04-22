#ifndef CUBE_CAMERA_HPP
#define CUBE_CAMERA_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace cube {

    constexpr auto WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
    constexpr auto CAMERA_SPEED = 25.f;
    constexpr auto MOUSE_SENSITIVITY = 0.1f;

    class Camera final {
    public:
        explicit Camera(const glm::vec3& pos = {0,0,0});

        [[nodiscard]] glm::mat4 getViewMatrix() const;

        [[nodiscard]] glm::vec3 getPosition() const;
        [[nodiscard]] glm::vec3 getFront() const;
        [[nodiscard]] glm::vec3 getRight() const;

        [[nodiscard]] glm::vec2 getDirection() const;

        void move(const glm::vec3& position);
        void rotate(const glm::vec2& rotation);

    private:
        void updateVectors();

        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_right;
        glm::vec2 m_direction;
    };

}

#endif //CUBE_CAMERA_HPP
