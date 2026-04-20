#ifndef CUBE_RENDERER_HPP
#define CUBE_RENDERER_HPP

#define GLM_ENABLE_EXPERIMENTAL

#include <unordered_map>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>

#include "Shader.hpp"
#include "cube/task/Meshing.hpp"

namespace cube {

    struct ChunkRenderable final {
        uint32_t vao = 0;
        uint32_t vbo = 0;
        uint32_t ebo = 0;
        uint32_t indexCount = 0;
        bool isReady = false;

        void cleanup() const;
    };

    class Renderer final {
    public:
        Renderer();
        ~Renderer();

        void push(const MeshResult& result);
        void evict(const glm::ivec3& pos);
        void draw(const glm::mat4& view, const glm::mat4& proj);

    private:
        uint32_t m_textures;
        Shader m_shader;
        std::vector<ChunkRenderable> m_freePool;
        std::unordered_map<glm::ivec3, ChunkRenderable> m_chunks;
    };

}

#endif //CUBE_RENDERER_HPP
