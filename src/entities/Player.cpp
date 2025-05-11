#include "cube/entities/Player.hpp"

namespace cube {

    Player::Player() = default;
    Player::~Player() = default;

    void Player::setPosition(const glm::vec3& pos) {
        m_camera.setPosition(pos);
    }

    void Player::rotate(const float dx, const float dy) {
        m_camera.rotate(dx,dy);
    }

    void Player::move(const uint8_t dir, const float dt) {
        m_camera.move(dir, dt);
    }

    void Player::setRotation(const float angle_x, const float angle_y) {
        m_camera.setRotation({angle_x,angle_y});
    }

    glm::vec3 Player::getPosition() const{
        return m_camera.getPosition();
    }

    glm::vec2 Player::getRotation() const{
        return m_camera.getRotation();
    }

    Camera Player::getCamera() const {
        return m_camera;
    }
}
