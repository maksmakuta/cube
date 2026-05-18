#include "cube/graphics/Renderer.hpp"

#include <algorithm>
#include <fstream>
#include <ranges>
#include <sstream>

#include <glad/glad.h>

#include "cube/core/Config.hpp"

namespace cube {

    Renderer::Renderer() {
        loadShaders();
    }

    Renderer::~Renderer() {
        for (auto &obj: m_objects | std::views::values) {
            if (obj.solid_vao) glDeleteVertexArrays(1, &obj.solid_vao);
            if (obj.solid_vbo) glDeleteBuffers(1, &obj.solid_vbo);
            if (obj.solid_ebo) glDeleteBuffers(1, &obj.solid_ebo);

            if (obj.blend_vao) glDeleteVertexArrays(1, &obj.blend_vao);
            if (obj.blend_vbo) glDeleteBuffers(1, &obj.blend_vbo);
            if (obj.blend_ebo) glDeleteBuffers(1, &obj.blend_ebo);
        }
    }

    void Renderer::uploadChunk(const glm::ivec3& pos, const Mesh& mesh) {
        if (const auto it = m_objects.find(pos); it != m_objects.end()) {
            const GLObject& oldObj = it->second;
            if (oldObj.solid_vao) glDeleteVertexArrays(1, &oldObj.solid_vao);
            if (oldObj.solid_vbo) glDeleteBuffers(1, &oldObj.solid_vbo);
            if (oldObj.solid_ebo) glDeleteBuffers(1, &oldObj.solid_ebo);
            if (oldObj.blend_vao) glDeleteVertexArrays(1, &oldObj.blend_vao);
            if (oldObj.blend_vbo) glDeleteBuffers(1, &oldObj.blend_vbo);
            if (oldObj.blend_ebo) glDeleteBuffers(1, &oldObj.blend_ebo);
        }

        GLObject newObj{};

        if (!mesh.solid.vertices.empty()) {
            glGenVertexArrays(1, &newObj.solid_vao);
            glGenBuffers(1, &newObj.solid_vbo);
            glGenBuffers(1, &newObj.solid_ebo);

            glBindVertexArray(newObj.solid_vao);
            glBindBuffer(GL_ARRAY_BUFFER, newObj.solid_vbo);
            glBufferData(GL_ARRAY_BUFFER, mesh.solid.vertices.size() * sizeof(Vertex), mesh.solid.vertices.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newObj.solid_ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.solid.indices.size() * sizeof(uint32_t), mesh.solid.indices.data(), GL_STATIC_DRAW);

            glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, 0, static_cast<void *>(nullptr));
            glEnableVertexAttribArray(0);

            newObj.solid_count = static_cast<uint32_t>(mesh.solid.indices.size());
        }

        if (!mesh.transparent.vertices.empty()) {
            glGenVertexArrays(1, &newObj.blend_vao);
            glGenBuffers(1, &newObj.blend_vbo);
            glGenBuffers(1, &newObj.blend_ebo);

            glBindVertexArray(newObj.blend_vao);
            glBindBuffer(GL_ARRAY_BUFFER, newObj.blend_vbo);
            glBufferData(GL_ARRAY_BUFFER, mesh.transparent.vertices.size() * sizeof(Vertex), mesh.transparent.vertices.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newObj.blend_ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.transparent.indices.size() * sizeof(uint32_t), mesh.transparent.indices.data(), GL_STATIC_DRAW);

            glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, 0, static_cast<void *>(nullptr));
            glEnableVertexAttribArray(0);

            newObj.blend_count = static_cast<uint32_t>(mesh.transparent.indices.size());
        }

        m_objects[pos] = newObj;

        if (std::ranges::find(m_list, pos) == m_list.end()) {
            m_list.push_back(pos);
        }
    }

    void Renderer::unloadChunks(const glm::ivec3& pos, const int render_distance) {
        auto it = m_objects.begin();
        while (it != m_objects.end()) {
            glm::ivec3 chunkPos = it->first;

            const auto distance = static_cast<int>(glm::floor(glm::length(glm::vec3(chunkPos - pos))));

            if (distance > render_distance) {
                GLObject& obj = it->second;
                if (obj.solid_vao) glDeleteVertexArrays(1, &obj.solid_vao);
                if (obj.solid_vbo) glDeleteBuffers(1, &obj.solid_vbo);
                if (obj.solid_ebo) glDeleteBuffers(1, &obj.solid_ebo);
                if (obj.blend_vao) glDeleteVertexArrays(1, &obj.blend_vao);
                if (obj.blend_vbo) glDeleteBuffers(1, &obj.blend_vbo);
                if (obj.blend_ebo) glDeleteBuffers(1, &obj.blend_ebo);

                m_list.erase(std::ranges::remove(m_list, chunkPos).begin(), m_list.end());

                it = m_objects.erase(it);
            } else {
                ++it;
            }
        }
    }

    void Renderer::render(const Camera& camera) {
        m_shader.use();
        m_shader.setMat4("u_Projection", camera.getProjection());
        m_shader.setMat4("u_View", camera.getViewMatrix());

        glm::vec3 cameraPos = camera.getPosition();

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);

        for (const auto& pos : m_list) {
            if (const GLObject& obj = m_objects[pos]; obj.solid_count > 0) {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos * CHUNK_SIZE));
                m_shader.setMat4("u_Model", model);

                glBindVertexArray(obj.solid_vao);
                glDrawElements(GL_TRIANGLES, obj.solid_count, GL_UNSIGNED_INT, 0);
            }
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);

        std::ranges::sort(m_list, [&cameraPos](const glm::ivec3& a, const glm::ivec3& b) {
            const float distA = glm::distance(cameraPos, glm::vec3(a * CHUNK_SIZE + 8));
            const float distB = glm::distance(cameraPos, glm::vec3(b * CHUNK_SIZE + 8));
            return distA > distB;
        });

        for (const auto& pos : m_list) {
            if (const GLObject& obj = m_objects[pos]; obj.blend_count > 0) {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos * CHUNK_SIZE));
                m_shader.setMat4("u_Model", model);

                glBindVertexArray(obj.blend_vao);
                glDrawElements(GL_TRIANGLES, obj.blend_count, GL_UNSIGNED_INT, 0);
            }
        }

        glDepthMask(GL_TRUE);
    }

    std::string readFile(const std::string& path) {
        if (std::ifstream ifs(path); ifs.is_open()) {
            return (std::stringstream() << ifs.rdbuf()).str();
        }
        return "";
    }

    void Renderer::loadShaders() {
        const auto vert = readFile("../assets/shaders/voxel.vert");
        const auto frag = readFile("../assets/shaders/voxel.frag");
        m_shader.compile(vert, frag);
    }
}
