#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace cube {

    constexpr auto CHUNK_WIDTH = 16;
    constexpr auto CHUNK_DEPTH = 16;
    constexpr auto CHUNK_HEIGHT = 128;
    constexpr auto CHUNK_SIZE = CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT;

    constexpr auto CAMERA_SPEED = 7.5f;
    constexpr auto CAMERA_SENSITIVITY = .1f;

}

#endif //CONSTANTS_HPP
