#include <random>

#include <cube/Cube.hpp>
#include <cube/utils/Logger.hpp>
#include <cube/data/Mesh.hpp>

#include "glad/glad.h"

namespace cube {

    constexpr auto DIST = 4;
    constexpr auto RM_DIST = DIST * 2;

    int randomSeed() {
        std::random_device dev;
        std::default_random_engine r(dev());
        const auto seed =  static_cast<int>(r());
        Log::info("Seed : {}", seed);
        return seed;
    }

    Cube::Cube() : m_world(randomSeed()) {
        m_meshingThread = std::jthread(&Cube::meshingLoop, this);
        m_camera.move({0,128,0});

        const auto initial = m_world.loadArea({0, 0, 0}, DIST);
        {
            std::lock_guard lock(m_taskMutex);
            for (auto& p : initial) m_meshTasks.push(p);
        }
        m_taskCV.notify_all();

        glDisable(GL_CULL_FACE);
    }

    Cube::~Cube() = default;

    void Cube::onDraw() {
        m_renderer.render(m_projection, m_camera.getViewMatrix());
    }

    void Cube::onUpdate(const float dt) {
        auto newPos = m_camera.getPosition();
        const auto buttons = SDL_GetKeyboardState(nullptr);
        if (buttons[SDL_SCANCODE_W]) newPos += m_camera.getFront() * dt * CAMERA_SPEED;
        if (buttons[SDL_SCANCODE_S]) newPos -= m_camera.getFront() * dt * CAMERA_SPEED;
        if (buttons[SDL_SCANCODE_A]) newPos -= m_camera.getRight() * dt * CAMERA_SPEED;
        if (buttons[SDL_SCANCODE_D]) newPos += m_camera.getRight() * dt * CAMERA_SPEED;
        if (buttons[SDL_SCANCODE_SPACE]) newPos += WORLD_UP * dt * CAMERA_SPEED;
        if (buttons[SDL_SCANCODE_LSHIFT]) newPos -= WORLD_UP * dt * CAMERA_SPEED;
        m_camera.move(newPos);

        glm::ivec3 currentChunkPos = {
            static_cast<int>(std::floor(newPos.x / 16.0f)),
            static_cast<int>(std::floor(newPos.y / 16.0f)),
            static_cast<int>(std::floor(newPos.z / 16.0f))
        };

        static glm::ivec3 lastChunkPos = {-99999, -99999, -99999};

        if (currentChunkPos.x != lastChunkPos.x || currentChunkPos.z != lastChunkPos.z) {
            m_world.unloadFarChunks(
               currentChunkPos,
               RM_DIST * RM_DIST,
               [this](const glm::ivec3& p) {
                   m_renderer.remove(p);
               }
            );

            const auto new_chunks = m_world.loadArea(currentChunkPos,DIST);
            if (!new_chunks.empty()) {
                std::lock_guard lock(m_taskMutex);
                for (auto& p : new_chunks) m_meshTasks.push(p);
                m_taskCV.notify_one();
            }

            lastChunkPos = currentChunkPos;
        }

        {
            std::lock_guard lock(m_resultMutex);
            while (!m_meshResults.empty()) {
                m_renderer.put(m_meshResults.front());
                m_meshResults.pop();
            }
            m_renderer.sortChunks(currentChunkPos);
        }
    }

    void Cube::onEvent(const SDL_Event& e) {
        if (e.type == SDL_EVENT_MOUSE_MOTION) {
            m_camera.rotate(glm::vec2{e.motion.xrel, -e.motion.yrel} * MOUSE_SENSITIVITY);
        }
        if (e.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
            const auto aspect = static_cast<float>(e.window.data1) / static_cast<float>(e.window.data2);
            m_projection = glm::perspective(glm::radians(45.f),aspect,0.1f,512.f);
        }
        if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            m_taskCV.notify_all();
            m_meshingThread.request_stop();
        }
    }

    void Cube::meshingLoop(const std::stop_token &stoken) {
        while (!stoken.stop_requested()) {
            glm::ivec3 pos;
            {
                std::unique_lock lock(m_taskMutex);
                m_taskCV.wait(lock, [&] {
                    return !m_meshTasks.empty() || stoken.stop_requested();
                });

                if (stoken.stop_requested()) break;

                pos = m_meshTasks.front();
                m_meshTasks.pop();
            }
            if (ChunkNeighbors neighbors = m_world.getNeighbors(pos); neighbors.isValid()) {
                RenderableMesh mesh = getMesh(neighbors, pos);
                std::lock_guard lock(m_resultMutex);
                m_meshResults.push(std::move(mesh));
            }else {
                Log::warn("Cannot mesh chunk [{},{},{}]: not valid", pos.x, pos.y, pos.z);
                //m_meshTasks.emplace(pos);
            }
        }
    }

}
