#ifndef CUBE_CUBE_HPP
#define CUBE_CUBE_HPP

#include <SDL3/SDL_events.h>

#include <cube/data/Chunk.hpp>
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
        void generateChunk(const glm::ivec3&);
        void meshChunk(const glm::ivec3&);

        int m_seed;
        Chunk m_chunk;
        Camera m_camera;
        Renderer m_renderer;
        glm::mat4 m_projection{1.0f};
    };

}

#endif //CUBE_CUBE_HPP
