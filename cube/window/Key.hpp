#ifndef CUBE_KEY_HPP
#define CUBE_KEY_HPP
#include <string>

namespace cube {

    enum class Key {
        Unknown = -1,

        Escape = 0,
        Shift = 1,
        Control = 2,
        Alt = 3,
        Space = 4,

        F1,
        F2,
        F3,
        F4,

        W,
        S,
        A,
        D,

        MouseLeft = 100,
        MouseRight,
    };

    std::string toString(const Key&);

}



#endif //CUBE_KEY_HPP