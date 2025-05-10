#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "cube/graphics/Camera.hpp"

namespace cube {

    class Player {
    public:
        Player();
        ~Player();

        void moveTo(const glm::vec3& pos);
        void rotate(float angle_x, float angle_y);

        void move(uint8_t dir = 0);
        void move(float dx, float dy);
        void update(float dt);

        [[nodiscard]] glm::vec3 getPosition() const;
        [[nodiscard]] glm::vec2 getRotation() const;

    private:
        Camera m_camera;
        uint8_t m_direction{0};
    };
}

#endif //PLAYER_HPP
