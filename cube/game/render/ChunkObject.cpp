#include "ChunkObject.hpp"

#include "glad/gl.h"

namespace cube {

    void ChunkObject::init(const int vertices, const int indices) {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        const auto vertices_size = static_cast<int>(vertices * sizeof(ChunkVertex));
        const auto indices_size = static_cast<int>(indices * sizeof(uint32_t));

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices_size ,nullptr, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size,nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_UNSIGNED_BYTE, GL_FALSE,sizeof(ChunkVertex), reinterpret_cast<void *>(offsetof(ChunkVertex, pos)));
        glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE,sizeof(ChunkVertex), reinterpret_cast<void *>(offsetof(ChunkVertex, nor)));
        glVertexAttribPointer(2, 2, GL_UNSIGNED_BYTE, GL_FALSE,sizeof(ChunkVertex), reinterpret_cast<void *>(offsetof(ChunkVertex, tex)));
        glVertexAttribIPointer(3, 1, GL_UNSIGNED_BYTE,sizeof(ChunkVertex), reinterpret_cast<void *>(offsetof(ChunkVertex, id)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);
    }

    void ChunkObject::update(const ChunkMesh& mesh) {
        const auto vertices_size = static_cast<int>(mesh.vertices.size() * sizeof(ChunkVertex));
        const auto indices_size = static_cast<int>(mesh.indices.size() * sizeof(uint32_t));
        count = static_cast<int>(mesh.indices.size());

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices_size ,mesh.vertices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size,mesh.indices.data(), GL_DYNAMIC_DRAW);
    }

    void ChunkObject::release() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        vao = ebo = vbo = 0;
    }

    ChunkObject toObject(const ChunkMesh& mesh) {
        auto object = ChunkObject();
        object.init();
        object.update(mesh);
        return object;
    }

}
