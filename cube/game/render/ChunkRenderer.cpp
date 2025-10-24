#include "ChunkRenderer.hpp"

#include "graphics/TextureBuilder.hpp"
#include "utils/AssetsPaths.hpp"

namespace cube {
    ChunkRenderer::ChunkRenderer() {
        m_shader.fromName("render3d");
        m_textures = TextureBuilder()
            .generateMipmaps(true)
            .setWrap(TextureWrap::Repeat)
            .setFilter(TextureFilter::Nearest)
            .buildArray(getTexture("blocks/"));
    }

    ChunkRenderer::~ChunkRenderer() {
        m_shader.release();
        m_textures.release();
    }

    bool ChunkRenderer::exists(const ChunkPos& pos) const {
        return m_chunks.contains(pos);
    }

    void ChunkRenderer::add(const ChunkPos& pos, const ChunkObject& obj) {
        m_chunks.insert_or_assign(pos,obj);
    }

    void ChunkRenderer::remove(const ChunkPos& pos) {
        m_chunks.erase(pos);
    }

    void ChunkRenderer::resize(const glm::vec2& v, const float fov) {
        m_projection = glm::perspective(glm::radians(fov), v.x / v.y, 0.1f, CHUNK_SIZE.x * 8.f);
        const auto size = glm::ivec2(v);
        glViewport(0, 0, size.x, size.y);
    }

    void ChunkRenderer::draw(const glm::mat4& view) {
        if (m_chunks.empty()) return;

        m_shader.use();
        m_shader.setMat4("view", view);
        m_shader.setMat4("proj", m_projection);

        m_textures.bind();
        m_shader.setInt("textures", 0);

        for (const auto& [pos, obj] : m_chunks) {
            if (obj.count <= 0) continue;
            m_shader.setMat4("model", glm::translate(glm::mat4(1.0f), {CHUNK_SIZE.x * pos.x,0,CHUNK_SIZE.z * pos.y}));
            glBindVertexArray(obj.vao);
            glDrawElements(GL_TRIANGLES, obj.count, GL_UNSIGNED_INT, nullptr);
        }
    }

}
