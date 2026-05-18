#ifndef CUBE_CUBE_HPP
#define CUBE_CUBE_HPP

#include <SDL3/SDL_events.h>

#include "data/Generator.hpp"
#include "data/World.hpp"
#include "graphics/Camera.hpp"
#include "graphics/Renderer.hpp"
#include "utils/ConcurrentQueue.hpp"
#include "utils/ThreadPool.hpp"

namespace cube {

    struct MeshResult {
        glm::ivec3 pos;
        Mesh mesh;
    };

    class Cube final {
    public:
        Cube();

        void onUpdate(float dt);
        void onRender();
        void onEvent(const SDL_Event& event);

    private:
        World m_world;
        Camera m_camera;
        Renderer m_renderer;
        Generator m_generator;
        ThreadPool m_gen_pool;
        ThreadPool m_mesh_pool;
        ConcurrentQueue<MeshResult> m_meshQueue;
    };

}

#endif //CUBE_CUBE_HPP
