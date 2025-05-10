#include "cube/graphics/renderer/VoxelRenderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "cube/core/Constants.hpp"
#include "cube/utils/AssetsUtils.hpp"
#include "glad/gl.h"


namespace cube {

    VoxelRenderer::VoxelRenderer() = default;
    VoxelRenderer::~VoxelRenderer() = default;

    void VoxelRenderer::onCreate(){
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
    }

    void VoxelRenderer::onResize(const int w, const int h){
        const auto aspect = static_cast<float>(w) / static_cast<float>(h);
        m_projection = glm::perspective(glm::radians(70.f),aspect, 0.f, CHUNK_DEPTH * 8.f);
    }

    void VoxelRenderer::drawCube() {

    }

    void VoxelRenderer::drawChunk(const Chunk&, const glm::vec2&) {

    }

}
