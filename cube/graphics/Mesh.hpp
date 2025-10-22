#ifndef CUBE_MESH_HPP
#define CUBE_MESH_HPP

#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "models/world/Chunk.hpp"

namespace cube {

    struct CubeVertex {
        glm::vec3 pos;
        glm::vec3 nor;
        glm::vec2 tex;
        uint32_t   id;
    };

    struct Mesh {
        std::vector<CubeVertex> vertices;
        std::vector<uint32_t> indices;

        void clear();
        void reserve(size_t v, size_t i);
    };

    struct Renderable {
        uint32_t vao{0};
        uint32_t vbo{0};
        uint32_t ebo{0};
        int count{0};
        glm::mat4 model{1.f};

        void init(int vertices = 2048, int indices = 4096);
        void update(const Mesh&);
        void release() const;
    };

    Mesh toMesh(const ChunkPtr&, const std::vector<ChunkPtr>& = {});
    Renderable toRenderable(const glm::ivec2&, const ChunkPtr&, const std::vector<ChunkPtr>& = {});

}

#endif //CUBE_MESH_HPP