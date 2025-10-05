#include "Event.hpp"

#include <sstream>

#include "utils/LambdaVisitor.hpp"

namespace cube {

    std::string toString(const Event& e) {
        std::stringstream ss;
        std::visit(LambdaVisitor{
            [&ss](const ResizeEvent& r) {
                ss << "ResizeEvent: [" << r.width << "x" << r.height << ']';
            },
            [&ss](const KeyEvent& r) {
                ss << "KeyEvent: [" << toString(r.key) << ", " << r.pressed << "]";
            },
            [&ss](const MouseEvent& r) {
                ss << "MouseEvent: [" << r.x << ", " << r.y << "]";
            },
            [&ss](const ScrollEvent& r) {
                ss << "ScrollEvent: [" << r.dx << ", " << r.dy << "]";
            },
            [&ss](const InputEvent& r) {
                ss << "InputEvent: [" << r.code << "]";
            },
        }, e);
        return ss.str();
    }

}

