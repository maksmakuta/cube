#include "cube/graphics/VoxelItem.hpp"

#include "glad/gl.h"

namespace cube {
    void VoxelRenderable::load() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
    }

    void VoxelRenderable::upload(const std::vector<Vertex3D> &vertices, const std::vector<unsigned> &indices) {
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<int>(vertices.size() * sizeof(Vertex3D)), vertices.data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(indices.size() * sizeof(uint32_t)), indices.data(),
                     GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                              reinterpret_cast<void *>(offsetof(Vertex3D, tex)));
        glEnableVertexAttribArray(1);

        glVertexAttribIPointer(2, 1, GL_INT, sizeof(Vertex3D), reinterpret_cast<void *>(offsetof(Vertex3D, ao)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        count = static_cast<int>(indices.size());
    }

    void VoxelRenderable::unload() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }
}
