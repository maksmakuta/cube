#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#define constval constexpr auto

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace cube {

    constval TITLE = "Cube";

    constval CHUNK_WIDTH = 16;
    constval CHUNK_DEPTH = 16;
    constval CHUNK_HEIGHT = 128;
    constval CHUNK_WATER = CHUNK_HEIGHT / 2;
    constval CHUNK_SIZE = CHUNK_DEPTH * CHUNK_HEIGHT * CHUNK_WIDTH;

    constval CAMERA_SPEED = 16.f;
    constval CAMERA_SENSITIVITY = 0.1f;
    constval CAMERA_WORLD_UP = glm::vec3{0,1,0};

    constval TICK = 0.200f;
    constval FPS_TICK = 0.500f;
}

#endif //CONSTANTS_HPP
