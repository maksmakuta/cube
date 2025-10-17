#ifndef CUBE_CHUNKRENDERER_HPP
#define CUBE_CHUNKRENDERER_HPP

#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/hash.hpp>

#include "Shader.hpp"
#include "TextureArray.hpp"

namespace cube {

    struct ChunkRenderable {
        uint32_t vao;
        uint32_t vbo;
        uint32_t ebo;
        glm::mat4 model;
    };

    class ChunkRenderer {
    public:
        ChunkRenderer();
        ~ChunkRenderer();

        void resize(const glm::vec2&, float fov = 70.f);
        void render(const glm::mat4& view);

        void addChunk(const glm::ivec2&, const ChunkRenderable&);
        void updateChunk(const glm::ivec2&, const ChunkRenderable&);
        void removeChunk(const glm::ivec2&);

    private:
        Shader m_shader;
        TextureArray m_textures;
        glm::mat4 m_projection{1.f};
        std::unordered_map<glm::ivec2, ChunkRenderable> m_chunks;
    };

}

#endif //CUBE_CHUNKRENDERER_HPP