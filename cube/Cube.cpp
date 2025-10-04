#include "Cube.hpp"

namespace cube {

    Cube::Cube() : m_window(std::make_unique<Window>()){

    }

    bool Cube::isRunning() const {
        return !m_window->isClosed();
    }

    void Cube::update() {
        m_window->update();
    }

    void Cube::render() {
        m_window->swapBuffers();
    }

}