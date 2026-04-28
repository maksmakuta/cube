#ifndef CUBE_CUBE_HPP
#define CUBE_CUBE_HPP

#include <unordered_map>
#include <SDL3/SDL_events.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "core/ChunkState.hpp"
#include "graphics/Camera.hpp"

namespace cube {

    class Cube {
    public:
        Cube();

        void onUpdate(float dt);
        void onDraw();
        void onEvent(const SDL_Event& event);

    private:
        Camera m_camera;
        std::unordered_map<glm::ivec3, ChunkState> m_states;
    };

}

#endif //CUBE_CUBE_HPP
