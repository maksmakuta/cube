#ifndef CUBE_CAMERA_HPP
#define CUBE_CAMERA_HPP

#include <unordered_map>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "window/Event.hpp"

namespace cube {

    class Camera {
    public:
        Camera() = default;

        void update(float dt);
        void onEvent(const Event&);

        glm::mat4 getMatrix() const;
        glm::vec3 getPosition() const;
        glm::vec2 getRotation() const;
        float getFOV() const;

        void setPosition(const glm::vec3&);
        void setRotation(const glm::vec2&);
        void setFOV(float);
    private:
        void updateDirection();

        glm::vec3 m_position{0};
        glm::vec3 m_front{0,0,1};
        glm::vec3 m_up{0,1,0};
        glm::vec2 m_rotation{0};
        glm::vec2 m_move{0};
        float m_fov = 45.f;
        std::unordered_map<Key, bool> m_keyState;
    };

}

#endif //CUBE_CAMERA_HPP