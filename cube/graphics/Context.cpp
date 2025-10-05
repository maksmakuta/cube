#include "Context.hpp"

#include "glad/gl.h"

namespace cube {

    void Context::setRenderer(const std::shared_ptr<Renderer2D>& r) {
        m_2d_renderer = r;
    }

    void Context::setRenderer(const std::shared_ptr<Renderer3D>& r) {
        m_3d_renderer = r;
    }

    std::shared_ptr<Renderer2D> Context::get2DRenderer() const {
        return m_2d_renderer;
    }

    std::shared_ptr<Renderer3D> Context::get3DRenderer() const {
        return m_3d_renderer;
    }

    void clear(const Color& c) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(c.rf(), c.gf(), c.bf(), c.af());
    }
}
