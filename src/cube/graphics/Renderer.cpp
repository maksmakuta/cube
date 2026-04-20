#include <cube/graphics/Renderer.hpp>

#include "glad/glad.h"

namespace cube {

    void ChunkRenderable::cleanup() const {
        if (vao) glDeleteVertexArrays(1, &vao);
        if (vbo) glDeleteBuffers(1, &vbo);
        if (ebo) glDeleteBuffers(1, &ebo);
    }

    Renderer::Renderer() : m_shader("../assets/shaders/cube.vert", "../assets/shaders/cube.frag") {
        // Load your Texture Array here
        m_textures = 0;
    }

    Renderer::~Renderer() {
        for (auto& [pos, renderable] : m_chunks) renderable.cleanup();
        for (auto& renderable : m_freePool) renderable.cleanup();
    }


    void update_renderable(ChunkRenderable& r, const MeshResult& res) {
        glNamedBufferData(r.vbo, res.vertices.size() * sizeof(Vertex), res.vertices.data(), GL_STATIC_DRAW);
        glNamedBufferData(r.ebo, res.indices.size() * sizeof(uint32_t), res.indices.data(), GL_STATIC_DRAW);
        r.indexCount = static_cast<uint32_t>(res.indices.size());
        r.isReady = true;
    }

    ChunkRenderable create_new_renderable() {
        ChunkRenderable r;
        glCreateBuffers(1, &r.vbo);
        glCreateBuffers(1, &r.ebo);
        glCreateVertexArrays(1, &r.vao);

        glVertexArrayVertexBuffer(r.vao, 0, r.vbo, 0, sizeof(Vertex));
        glVertexArrayElementBuffer(r.vao, r.ebo);
        glEnableVertexArrayAttrib(r.vao, 0);
        glVertexArrayAttribIFormat(r.vao, 0, 1, GL_UNSIGNED_INT, 0);
        glVertexArrayAttribBinding(r.vao, 0, 0);

        return r;
    }

    void Renderer::push(const MeshResult& result) {
        if (m_chunks.contains(result.pos)) {
            update_renderable(m_chunks[result.pos], result);
            return;
        }

        ChunkRenderable renderable;
        if (!m_freePool.empty()) {
            renderable = m_freePool.back();
            m_freePool.pop_back();
        } else {
            renderable = create_new_renderable();
        }

        update_renderable(renderable, result);
        m_chunks[result.pos] = renderable;
    }

    void Renderer::evict(const glm::ivec3& pos) {
        if (const auto it = m_chunks.find(pos); it != m_chunks.end()) {
            it->second.isReady = false;
            m_freePool.push_back(it->second);
            m_chunks.erase(it);
        }
    }

    void Renderer::draw(const glm::mat4& view, const glm::mat4& proj) {
        m_shader.use();
        m_shader.setMat4("view", view);
        m_shader.setMat4("projection", proj);

        for (auto& [pos, proxy] : m_chunks) {
            if (!proxy.isReady || proxy.indexCount == 0) continue;

            // 1. Frustum Culling Check (Crucial for performance)
            // if (!camera.frustum.contains(pos * 16, pos * 16 + 16)) continue;

            // 2. Set Model Matrix (Local to World)
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos * 16));
            m_shader.setMat4("model", model);

            // 3. Draw
            glBindVertexArray(proxy.vao);
            glDrawElements(GL_TRIANGLES, proxy.indexCount, GL_UNSIGNED_INT, nullptr);
        }
    }

}
