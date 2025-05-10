#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/mat4x4.hpp>

namespace cube {

    enum MoveDirection {
        Forward     = 1 << 1,
        Backward    = 1 << 2,
        Left        = 1 << 3,
        Right       = 1 << 4,
        Up          = 1 << 5,
        Down        = 1 << 6,
    };

    class Camera {
    public:
        explicit Camera(const glm::vec3& pos = {0,0,0});
        ~Camera();

        void setPosition(const glm::vec3&);
        void setRotation(const glm::vec2&);

        void move(uint8_t dir, float dt);
        void rotate(float dx, float dy);

        [[nodiscard]] glm::vec3 getPosition() const;
        [[nodiscard]] glm::vec2 getRotation() const;
        [[nodiscard]] glm::mat4 getView() const;

    private:
        void update();

        glm::vec3 m_position{0};
        glm::vec3 m_front{0}, m_right{0}, m_up{0};
        glm::vec2 m_rotation{0};
    };

}

#endif //CAMERA_HPP
