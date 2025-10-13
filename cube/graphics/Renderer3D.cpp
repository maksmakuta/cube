#include "Renderer3D.hpp"

namespace cube {

    Renderer3D::Renderer3D() {
        m_state.init();
    }

    Renderer3D::~Renderer3D() {
        m_state.release();
    }


}
