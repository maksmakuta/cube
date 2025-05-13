#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#define constval constexpr auto

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace cube {

    constval TITLE = "Cube";
    constval RENDER_DIST = 8.f;

    constval CHUNK_WIDTH = 16;
    constval CHUNK_DEPTH = 16;
    constval CHUNK_HEIGHT = 128;
    constval CHUNK_SIZE = CHUNK_DEPTH * CHUNK_HEIGHT * CHUNK_WIDTH;
    constval HEIGHT_WATER = CHUNK_HEIGHT * 0.67f;
    constval HEIGHT_GROUND = CHUNK_HEIGHT * 0.7f;

    constval ATLAS_SIZE = glm::vec2{160,256};
    constval TILE_SIZE = 16.f;
    constval TILES = ATLAS_SIZE / TILE_SIZE;

    constval CAMERA_SPEED = 16.f;
    constval CAMERA_SENSITIVITY = 0.1f;
    constval CAMERA_WORLD_UP = glm::vec3{0,1,0};

    constval TICK = 1.f / 5.f;
}

#endif //CONSTANTS_HPP
