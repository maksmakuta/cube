#include "cube/Cube.hpp"

#include "cube/utils/Log.hpp"

namespace cube {

    Cube::Cube(const int argc, char** argv): m_last_tick(0.f) {
        for (auto i = 0; i < argc; i++) {
            info("args[{}]: {}", i, argv[i]);
        }
    }

    void Cube::onUpdate(const float dt) {
        m_last_tick += dt;
        if (m_last_tick >= 1.f) {
            debug("FPS: {}", 1.0f / dt);
            m_last_tick = 0.f;
        }
    }

    void Cube::onDraw() {

    }

    void Cube::onEvent(const SDL_Event& event) {

    }

}
