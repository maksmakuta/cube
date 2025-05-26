#include "cube/graphics/VoxelRenderer.hpp"

#include <iostream>

#include "cube/core/Constants.hpp"
#include "cube/utils/Utils.hpp"
#include "glad/gl.h"

namespace cube {
    VoxelRenderer::VoxelRenderer() = default;

    VoxelRenderer::~VoxelRenderer() = default;

    void VoxelRenderer::onCreate() {
        m_shader.load(
            getAsset("/shaders/cube.vert"),
            getAsset("/shaders/cube.frag")
        );

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        glGenBuffers(1, &indirectBuffer);
        glGenBuffers(1, &instanceBuffer);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                              reinterpret_cast<void *>(offsetof(Vertex3D, col)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(2);
        glVertexAttribDivisor(2, 1);

        glBindVertexArray(0);
    }

    void VoxelRenderer::onClear() {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &indirectBuffer);
        glDeleteBuffers(1, &instanceBuffer);
        glDeleteVertexArrays(1, &vao);
        m_shader.unload();
    }

    void VoxelRenderer::onDraw(const glm::mat4 &view) {
        if (commandCount == 0) {
            return;
        }

        glEnable(GL_CULL_FACE);

        m_shader.use();
        m_shader.setMat4("proj", m_proj);
        m_shader.setMat4("view", view);

        glBindVertexArray(vao);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectBuffer);
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, commandCount, 0);
        glDisable(GL_CULL_FACE);

    }

    void VoxelRenderer::onTick(ThreadPool &pool, const World &world) {
        for (const auto &i: world.getChunks()) {
            const auto c = world.getChunk(i);
            if (c && !m_mesh_cache.contains(i)) {
                m_mesh_cache[i] = toMesh(c);
            }
        }

        std::vector<Vertex3D> allVertices;
        std::vector<uint32_t> allIndices;
        std::vector<DrawElementsIndirectCommand> commands;
        std::vector<glm::vec3> positions;

        GLuint baseVertex = 0;
        GLuint firstIndex = 0;
        GLuint baseInstance = 0;

        for (const auto &i: world.getChunks()) {
            if (!m_mesh_cache.contains(i)) {
                continue;
            }
            positions.push_back(glm::ivec3{i.x, 0, i.y} * CHUNK_ORIGIN);
            const auto &mesh = m_mesh_cache[i];
            if (mesh.vertices.empty() || mesh.indices.empty()) {
                continue;
            }
            allVertices.insert(allVertices.end(), mesh.vertices.begin(), mesh.vertices.end());
            allIndices.insert(allIndices.end(), mesh.indices.begin(), mesh.indices.end());

            DrawElementsIndirectCommand cmd = {
                .count = static_cast<GLuint>(mesh.indices.size()),
                .instanceCount = 1,
                .firstIndex = firstIndex,
                .baseVertex = baseVertex,
                .baseInstance = baseInstance
            };

            commands.push_back(cmd);

            baseVertex += static_cast<GLuint>(mesh.vertices.size());
            firstIndex += static_cast<GLuint>(mesh.indices.size());
            baseInstance++;
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, allVertices.size() * sizeof(Vertex3D), allVertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, allIndices.size() * sizeof(uint32_t), allIndices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectBuffer);
        glBufferData(GL_DRAW_INDIRECT_BUFFER, commands.size() * sizeof(DrawElementsIndirectCommand), commands.data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);

        commandCount = static_cast<GLsizei>(commands.size());
    }

    void VoxelRenderer::onResize(const int w, const int h) {
        const auto aspect = static_cast<float>(w) / static_cast<float>(h);
        m_proj = glm::perspective(glm::radians(45.f), aspect, 0.01f, RENDER_DIST * CHUNK_HEIGHT * 1.25f);
    }
}
