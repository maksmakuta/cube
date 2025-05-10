#include "cube/entities/Player.hpp"

namespace cube {

    Player::Player() = default;
    Player::~Player() = default;

    void Player::moveTo(const glm::vec3& pos) {
        m_camera.setPosition(pos);
    }

    void Player::rotate(float angle_x, float angle_y) {
        m_camera.setRotation({angle_x,angle_y});
    }

    void Player::move(const uint8_t dir) {
        m_direction = dir;
    }

    void Player::move(const float dx, const float dy) {
        m_camera.rotate(dx,dy);
    }

    void Player::update(const float dt) {
        m_camera.move(m_direction, dt);
    }

    glm::vec3 Player::getPosition() const{
        return m_camera.getPosition();
    }

    glm::vec2 Player::getRotation() const{
        return m_camera.getRotation();
    }

}