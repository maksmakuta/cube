#include "cube/graphics/renderer/VoxelRenderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "cube/core/Constants.hpp"
#include "cube/graphics/renderer/Vertex.hpp"
#include "cube/utils/AssetsUtils.hpp"
#include "glad/gl.h"

namespace cube {

    VoxelRenderer::VoxelRenderer() = default;
    VoxelRenderer::~VoxelRenderer() = default;

    void VoxelRenderer::onCreate(){
        m_atlas.load(getAsset("/textures/atlas.png"));

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, 256 * sizeof(Vertex3D), nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), reinterpret_cast<void *>(offsetof(Vertex3D, tex)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_shader.load(
            getAsset("/shaders/cube.vert"),
            getAsset("/shaders/cube.frag")
        );
    }

    void VoxelRenderer::onClear(){
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        m_shader.unload();
        m_atlas.unload();
    }

    void VoxelRenderer::onResize(const int w, const int h){
        const auto aspect = static_cast<float>(w) / static_cast<float>(h);
        m_projection = glm::perspective(glm::radians(90.f),aspect, 0.f, CHUNK_DEPTH * 8.f);
    }

    void VoxelRenderer::draw(const std::vector<Vertex3D>& mesh, const glm::vec2& pos, const glm::mat4& view) const {
        glEnable(GL_DEPTH);

        m_atlas.bind(0);

        m_shader.use();
        m_shader.setMat4("proj",m_projection);
        m_shader.setMat4("view",view);
        m_shader.setMat4("model",glm::translate(glm::mat4{1.f},glm::vec3{pos.x * CHUNK_WIDTH, 0 , pos.y * CHUNK_DEPTH}));
        m_shader.setInt("atlas",0);
        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<int>(mesh.size() * sizeof(Vertex3D)), mesh.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0,static_cast<int>(mesh.size()));

    }

}
