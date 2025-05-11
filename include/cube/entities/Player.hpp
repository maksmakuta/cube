#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "cube/graphics/Camera.hpp"

namespace cube {

    class Player {
    public:
        Player();
        ~Player();

        void setPosition(const glm::vec3& pos);
        void setRotation(float angle_x, float angle_y);

        void move(uint8_t dir, float dt);
        void rotate(float dx, float dy);

        [[nodiscard]] glm::vec3 getPosition() const;
        [[nodiscard]] glm::vec2 getRotation() const;
        [[nodiscard]] Camera getCamera() const;

    private:
        Camera m_camera;
    };
}

#endif //PLAYER_HPP
