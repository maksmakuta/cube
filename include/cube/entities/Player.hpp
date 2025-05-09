#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "cube/graphics/Camera.hpp"

namespace cube {

    class Player {
    public:
        Player();
        ~Player();

        void move();
        void update();

    private:
        Camera m_camera;
    };
}

#endif //PLAYER_HPP
