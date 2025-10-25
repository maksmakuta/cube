#include "Key.hpp"

namespace cube {

    std::string toString(const Key& k) {
        switch (k) {
            case Key::Escape:
                return "ESC";
            case Key::Shift:
                return "Shift";
            case Key::Control:
                return "Ctrl";
            case Key::Alt:
                return "Alt";
            case Key::Space:
                return "Space";
            case Key::W:
                return "W";
            case Key::S:
                return "S";
            case Key::A:
                return "A";
            case Key::D:
                return "D";
            case Key::MouseLeft:
                return "MouseL";
            case Key::MouseRight:
                return "MouseR";
            case Key::F1:
                return "F1";
            case Key::F2:
                return "F2";
            case Key::F3:
                return "F3";
            case Key::F4:
                return "F4";
            default:
                return "Unknown";
        }
    }

}
