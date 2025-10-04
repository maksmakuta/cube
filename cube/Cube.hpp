#ifndef CUBE_CUBE_HPP
#define CUBE_CUBE_HPP

#include <memory>

#include "engine/Engine.hpp"
#include "graphics/Window.hpp"

namespace cube {

    class Cube {
    public:
        Cube();

        [[nodiscard]] bool isRunning() const;

        void update();
        void render();

    private:
        std::unique_ptr<Window> m_window;
        std::unique_ptr<Engine> m_engine;

    };

}

#endif //CUBE_CUBE_HPP