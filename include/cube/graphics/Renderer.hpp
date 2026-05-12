#ifndef CUBE_RENDERER_HPP
#define CUBE_RENDERER_HPP

#include <cstdint>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "cube/graphics/Shader.hpp"

namespace cube {

    struct ChunkMesh;

    struct Renderable final {
        uint32_t vao;
        uint32_t vbo;
        uint32_t ebo;
        int count;
        glm::vec3 min;
        glm::vec3 max;
    };

    class Renderer final {
    public:
        Renderer();
        ~Renderer();

        bool contains(const glm::ivec3&) const;
        void remove(const glm::ivec3&);
        void push(const glm::ivec3&, const ChunkMesh&);
        void draw(const glm::mat4& view, const glm::mat4& projection);

        int clearChunks(const glm::ivec3& pos, int dist);

    private:
        void loadTextures(const glm::ivec2& tileSize = glm::ivec2(32, 32));

        uint32_t m_textures;
        Shader m_shader;
        std::unordered_map<glm::ivec3, Renderable> m_meshes;
    };

}

#endif //CUBE_RENDERER_HPP
