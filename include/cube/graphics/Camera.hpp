#ifndef CUBE_CAMERA_HPP
#define CUBE_CAMERA_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace cube {

    class Camera final {
    public:
        explicit Camera(const glm::vec3& position = {0,0,0});

        [[nodiscard]] glm::mat4 getViewMatrix() const;
        [[nodiscard]] glm::mat4 getProjMatrix(const glm::vec2& view, float render_dist) const;

        [[nodiscard]] glm::vec3 getForward() const;
        [[nodiscard]] glm::vec3 getRight() const;

        [[nodiscard]] glm::vec3 getPosition() const;
        [[nodiscard]] glm::vec2 getDirection() const;
        [[nodiscard]] float getFov() const;

        void setPosition(const glm::vec3& position);
        void setDirection(const glm::vec2& direction);
        void setFov(float fov);

        void applyDirection(const glm::vec2& direction);

    private:
        void update();

        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_right;
        glm::vec2 m_direction;
        float m_fov;
    };

}

#endif //CUBE_CAMERA_HPP
