#ifndef CUBE_CUBE_HPP
#define CUBE_CUBE_HPP

#include <memory>

#include "Generator.hpp"
#include "render/ChunkRenderer.hpp"
#include "window/Event.hpp"

namespace cube {

    class Cube {
    public:
        Cube();
        ~Cube();

        void onDraw();
        void onUpdate(float);
        void onEvent(const Event&);

    private:
        Camera m_camera;
        World m_world;
        Generator m_gen;
        std::unique_ptr<ChunkRenderer> m_renderer{nullptr};
    };

}

#endif //CUBE_CUBE_HPP