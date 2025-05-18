#ifndef FACES_HPP
#define FACES_HPP

#include <cstdint>

namespace cube {

    enum class Face : uint8_t{
        Top,
        Bottom,
        Left,
        Right,
        Near,
        Far
    };

}

#endif //FACES_HPP
