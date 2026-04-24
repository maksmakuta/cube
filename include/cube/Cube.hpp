#ifndef CUBE_CUBE_HPP
#define CUBE_CUBE_HPP

#include <condition_variable>
#include <queue>
#include <thread>
#include <SDL3/SDL_events.h>

#include <cube/data/World.hpp>
#include <cube/graphics/Camera.hpp>
#include <cube/graphics/Renderer.hpp>

namespace cube {

    class Cube final {
    public:
        Cube();
        ~Cube();

        void onDraw();
        void onUpdate(float dt);
        void onEvent(const SDL_Event&);

    private:

        void meshingLoop(const std::stop_token &);

        std::jthread m_meshingThread;

        std::queue<glm::ivec3> m_meshTasks;
        std::mutex m_taskMutex;
        std::condition_variable m_taskCV;

        std::queue<RenderableMesh> m_meshResults;
        std::mutex m_resultMutex;

        ///
        World m_world;
        Camera m_camera;
        Renderer m_renderer;
        glm::mat4 m_projection{1.0f};
    };

}

#endif //CUBE_CUBE_HPP
