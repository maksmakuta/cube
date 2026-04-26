#ifndef CUBE_RENDERER_HPP
#define CUBE_RENDERER_HPP

#define GLM_ENABLE_EXPERIMENTAL
#include <shared_mutex>
#include <glm/gtx/hash.hpp>
#include <glm/mat4x4.hpp>

#include <unordered_map>

#include <cube/graphics/RenderableMesh.hpp>

namespace cube {

    struct Renderable final {
        glm::ivec3 model;
        uint32_t vao;
        uint32_t vbo;
        uint32_t ebo;
        uint32_t count;
    };

    class Renderer final {
    public:
        Renderer();
        ~Renderer();

        void remove(const glm::ivec3& pos);
        void put(const RenderableMesh& mesh);
        void render(const glm::mat4& projection, const glm::mat4& view);

    private:
        void loadShader();
        void loadTextures();

        int m_projLoc{-1};
        int m_viewLoc{-1};
        int m_modelLoc{-1};

        uint32_t m_shader{0};
        uint32_t m_texture{0};
        std::shared_mutex m_trashMutex;
        std::vector<glm::ivec3> m_removed;
        std::unordered_map<glm::ivec3, Renderable> m_renderables;
    };

}

#endif //CUBE_RENDERER_HPP
