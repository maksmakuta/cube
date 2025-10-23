#ifndef CUBE_WORLDRENDERER_HPP
#define CUBE_WORLDRENDERER_HPP

#include "Shader.hpp"
#include "TextureArray.hpp"
#include "game/Generator.hpp"
#include "models/Camera.hpp"
#include "models/world/World.hpp"

namespace cube {

    class WorldRenderer {
    public:
        explicit WorldRenderer(int seed = 0);
        ~WorldRenderer();

        void update(const Camera&);
        void resize(const glm::vec2& size, float fov = 45.f);
        void render(const glm::mat4& view);

    private:
        Shader m_shader;
        TextureArray m_textures;

        World m_world;
        Generator m_generator;

        glm::mat4 m_projection{1.f};
    };

}

#endif //CUBE_WORLDRENDERER_HPP