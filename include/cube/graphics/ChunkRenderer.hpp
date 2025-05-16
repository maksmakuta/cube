#ifndef CHUNKRENDERER_HPP
#define CHUNKRENDERER_HPP

#include <vector>

#include "Texture.hpp"
#include "cube/graphics/Shader.hpp"
#include "cube/graphics/Vertex.hpp"
#include "cube/utils/ThreadPool.hpp"
#include "cube/world/World.hpp"

namespace cube {

    struct ChunkMesh {
        glm::ivec2 pos;
        std::vector<Vertex3D> vertices;
        std::vector<glm::uint> indices;
    };

    struct ChunkRenderable {
        glm::uint vao;
        glm::uint vbo;
        glm::uint ebo;
        int count;

        ChunkRenderable();
        void uploadMesh(const std::vector<Vertex3D> &vertexData, const std::vector<uint32_t> &indexData);
        void unload();
    };

    class ChunkRenderer {
    public:
        ChunkRenderer();
        ~ChunkRenderer();

        void onCreate();
        void onClear();

        void onResize(int w, int h);
        void onDraw(const glm::mat4& view);
        void onTick(ThreadPool& pool, World& world);

    private:
        std::mutex m_mesh_mutex;
        std::unordered_map<glm::ivec2,ChunkRenderable> m_chunks;
        std::vector<ChunkMesh> m_meshes;
        glm::mat4 m_projection{1.f};
        Texture m_atlas;
        Shader m_shader;
    };

}

#endif //CHUNKRENDERER_HPP
