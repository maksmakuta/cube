#ifndef CUBE_EVENT_HPP
#define CUBE_EVENT_HPP

#include <cstdint>
#include <string>
#include <variant>

#include "Key.hpp"

namespace cube {

    struct ResizeEvent {
        int width;
        int height;
    };

    struct KeyEvent {
        Key key;
        bool pressed;
    };

    struct MouseEvent {
        float x;
        float y;
    };

    struct ScrollEvent {
        float dx;
        float dy;
    };

    struct InputEvent {
        uint32_t code;
    };

    using Event = std::variant<ResizeEvent, KeyEvent, MouseEvent, ScrollEvent, InputEvent>;

    std::string toString(const Event&);

}

#endif //CUBE_EVENT_HPP