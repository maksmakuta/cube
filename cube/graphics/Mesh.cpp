#include "Mesh.hpp"

#include <array>
#include <glm/ext/matrix_transform.hpp>

#include "glad/gl.h"

namespace cube {

    void Renderable::init(const int vertices, const int indices){
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        const auto vertices_size = static_cast<int>(vertices * sizeof(CubeVertex));
        const auto indices_size = static_cast<int>(indices * sizeof(uint32_t));

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices_size ,nullptr, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size,nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,sizeof(CubeVertex), reinterpret_cast<void *>(offsetof(CubeVertex, pos)));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,sizeof(CubeVertex), reinterpret_cast<void *>(offsetof(CubeVertex, nor)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,sizeof(CubeVertex), reinterpret_cast<void *>(offsetof(CubeVertex, tex)));
        glVertexAttribIPointer(3, 1, GL_INT,sizeof(CubeVertex), reinterpret_cast<void *>(offsetof(CubeVertex, id)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);
        
    }

    void Renderable::update(const Mesh& mesh) {
        const auto vertices_size = static_cast<int>(mesh.vertices.size() * sizeof(CubeVertex));
        const auto indices_size = static_cast<int>(mesh.indices.size() * sizeof(uint32_t));
        count = static_cast<int>(mesh.indices.size());

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices_size ,mesh.vertices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size,mesh.indices.data(), GL_DYNAMIC_DRAW);
    }

    void Renderable::release() const {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }

    Renderable toRenderable(const ChunkPtr& c, const glm::ivec2& pos) {
        if (c == nullptr)
            return {};
        const auto mesh = toMesh(c);

        Renderable renderable{};
        renderable.model = glm::translate(glm::mat4{1.f}, glm::vec3{CHUNK_SIZE.x * pos.x, 0.f, CHUNK_SIZE.z * pos.y});
        renderable.init();
        renderable.update(mesh);
        return renderable;

    }

    Mesh toMesh(const ChunkPtr& c) {
        Mesh mesh{};
        if (!c) return mesh;

        for (int x = 0; x < CHUNK_SIZE.x; x++)
        for (int z = 0; z < CHUNK_SIZE.z; z++)
        for (int y = 0; y < CHUNK_SIZE.y; y++) {

        }

        return mesh;
    }

}
