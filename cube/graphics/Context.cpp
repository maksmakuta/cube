#include "Context.hpp"

#include "glad/gl.h"

namespace cube {

    void clear(const Color& c) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(c.rf(), c.gf(), c.bf(), c.af());
    }
}
