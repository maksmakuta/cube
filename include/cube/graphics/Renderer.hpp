#ifndef CUBE_RENDERER_HPP
#define CUBE_RENDERER_HPP

#include <unordered_map>
#include <glm/mat4x4.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace cube {

    struct Vertex final {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 texture;
    };

    struct RenderableMesh final {
        glm::ivec3 pos;
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };

    struct Renderable final {
        glm::mat4 model;
        uint32_t vao;
        uint32_t vbo;
        uint32_t ebo;
        uint32_t count;
    };

    class Renderer final {
    public:
        Renderer();
        ~Renderer();

        void put(const glm::ivec3& pos, const RenderableMesh& mesh);
        void render(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& cameraPos);

    private:
        void loadShader();
        void loadTextures();

        uint32_t m_shader{0};
        uint32_t m_texture{0};
        std::unordered_map<glm::ivec3, Renderable> m_renderables;
    };

}

#endif //CUBE_RENDERER_HPP
