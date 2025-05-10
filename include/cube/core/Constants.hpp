#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#define constval constexpr auto

#include <glm/vec3.hpp>

namespace cube {

    constval TITLE = "Cube";

    constval CHUNK_WIDTH = 32;
    constval CHUNK_DEPTH = 32;
    constval CHUNK_HEIGHT = 256;

    constval TILE_SIZE = 16;

    constval CAMERA_SPEED = 16.f;
    constval CAMERA_SENSITIVITY = 0.16f;
    constval CAMERA_WORLD_UP = glm::vec3{0,1,0};
}

#endif //CONSTANTS_HPP
