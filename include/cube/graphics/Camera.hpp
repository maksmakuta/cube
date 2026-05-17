#ifndef CUBE_CAMERA_HPP
#define CUBE_CAMERA_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace cube {

    class Camera final {
    public:
        explicit Camera(float fov_angle = 70.0f, const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f));

        [[nodiscard]] glm::mat4 getViewMatrix() const;
        [[nodiscard]] glm::mat4 getProjection() const;

        [[nodiscard]] glm::vec3 getPosition() const;
        [[nodiscard]] glm::vec3 getFront() const;
        [[nodiscard]] glm::vec3 getRight() const;
        [[nodiscard]] glm::vec3 getUp() const;

        [[nodiscard]] glm::vec2 getDirection() const;

        void rotate(const glm::vec2& delta);
        void resize(const glm::vec2& size);
        void moveTo(const glm::vec3& pos);
    private:
        void update();

        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_right;
        glm::vec3 m_up;
        glm::vec2 m_direction;
        glm::vec2 m_size;
        float m_fov;
    };

}

#endif //CUBE_CAMERA_HPP
