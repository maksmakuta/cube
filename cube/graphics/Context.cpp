#include "Context.hpp"

#include "glad/gl.h"

namespace cube {

    void Context::setRenderer(const std::shared_ptr<Renderer2D>& renderer) {
        m_2d_renderer = renderer;
    }

    std::shared_ptr<Renderer2D> Context::getRenderer2D() const {
        return m_2d_renderer;
    }

    void clear(const Color& c) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(c.rf(), c.gf(), c.bf(), c.af());
    }
}
