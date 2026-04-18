#ifndef CUBE_RENDERSYSTEM_HPP
#define CUBE_RENDERSYSTEM_HPP

#include <memory>
#include <unordered_map>
#include <glm/fwd.hpp>

#include "Camera.hpp"
#include "Shader.hpp"
#include "cube/data/World.hpp"
#include "glad/glad.h"

namespace cube {

    struct ChunkMesh {
        GLuint vao{0};
        GLuint vbo{0};
        GLuint ebo{0};

        GLsizei indices{0};
        bool dirty = true;

        void clear() {
            if (vao) glDeleteVertexArrays(1, &vao);
            if (vbo) glDeleteBuffers(1, &vbo);
            if (ebo) glDeleteBuffers(1, &ebo);
            vao = vbo = ebo = 0;
            indices = 0;
        }
    };

    ChunkMesh getMesh(const Chunk &chunk);

    class RenderSystem final {
    public:
        RenderSystem();
        ~RenderSystem() = default;

        // Syncs GPU meshes with World data
        void update(World& world);

        // Draws everything to the screen
        void render(const Camera& camera, const glm::mat4& projection);

    private:
        std::unordered_map<glm::ivec3, ChunkMesh> m_meshes;
        std::unique_ptr<Shader> m_shader;
        glm::uint32_t m_textureArray{0};

        void buildMesh(const glm::ivec3& pos, const Chunk& chunk, ChunkMesh& mesh);
        void initTextureArray();
    };

}

#endif //CUBE_RENDERSYSTEM_HPP
