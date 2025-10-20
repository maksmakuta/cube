#ifndef CUBE_WORLDRENDERER_HPP
#define CUBE_WORLDRENDERER_HPP

#include "Mesh.hpp"
#include "Shader.hpp"
#include "TextureArray.hpp"
#include "game/Generator.hpp"
#include "models/Camera.hpp"
#include "models/world/World.hpp"

namespace cube {

    class WorldRenderer {
    public:
        explicit WorldRenderer(uint32_t seed = 0);
        ~WorldRenderer();

        void update(const Camera&);
        void resize(const glm::vec2& size, float fov = 45.f);
        void render(const glm::mat4& view);

    private:
        World m_world;
        Shader m_shader;
        Generator m_generator;
        TextureArray m_textures;
        glm::mat4 m_projection{1.f};
        std::vector<Renderable> m_free_list;
        std::unordered_map<ChunkPos, Renderable> m_renderables;
    };

}

#endif //CUBE_WORLDRENDERER_HPP