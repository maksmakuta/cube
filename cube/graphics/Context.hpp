#ifndef CUBE_CONTEXT_HPP
#define CUBE_CONTEXT_HPP

#include "Color.hpp"

namespace cube {

    class Context {
    public:
        Context() = default;
        ~Context() = default;
    };

    void clear(const Color&);

}

#endif //CUBE_CONTEXT_HPP