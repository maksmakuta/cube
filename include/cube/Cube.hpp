#ifndef CUBE_HPP
#define CUBE_HPP

#include <thread>
#include <SDL3/SDL_events.h>

#include "data/Generator.hpp"
#include "data/World.hpp"
#include "graphics/Camera.hpp"
#include "graphics/Renderer.hpp"
#include "mesh/Mesher.hpp"
#include "utils/SafeQueue.hpp"

namespace cube {

    class Cube final {
    public:
        Cube();

        void onUpdate(float dt);
        void onDraw();
        void onEvent(const SDL_Event& event);

    private:
        bool isReadyForMesh(const glm::ivec3& pos) const;
        void workerLoop();

        int m_frame_count;
        float m_last_tick;

        glm::vec2 m_view;
        glm::ivec3 m_last_chunk;

        SafeQueue<glm::ivec3> m_gq;
        SafeQueue<glm::ivec3> m_mq;
        SafeQueue<std::pair<glm::ivec3, ChunkMesh>> m_rq;

        std::vector<std::jthread> m_workers;
        std::atomic<bool> m_running{true};

        World m_world;
        Camera m_camera;
        Renderer m_renderer;
        Generator m_generator;
    };

}

#endif //CUBE_HPP
