#ifndef CUBE_CUBE_HPP
#define CUBE_CUBE_HPP

#define GLM_ENABLE_EXPERIMENTAL

#include <unordered_map>

#include <SDL3/SDL_events.h>
#include <glm/gtx/hash.hpp>

#include "core/ChunkState.hpp"
#include "data/World.hpp"
#include "graphics/Camera.hpp"
#include "graphics/Renderer.hpp"

namespace cube {

    class Cube {
    public:
        Cube();

        void onUpdate(float dt);
        void onDraw();
        void onEvent(const SDL_Event& event);

    private:
        ChunkState getChunkState(const glm::ivec3& pos);

        World m_world;
        Camera m_camera;
        Renderer m_renderer;
        std::unordered_map<glm::ivec3, ChunkState> m_states;
    };

}

#endif //CUBE_CUBE_HPP
