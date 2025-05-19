#ifndef FACES_HPP
#define FACES_HPP

#include <cstdint>

namespace cube {

    enum class Face : uint8_t {
        Right,
        Left,
        Top,
        Bottom,
        Front,
        Back
    };

}

#endif //FACES_HPP
