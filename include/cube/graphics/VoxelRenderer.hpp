#ifndef VOXELRENDERER_HPP
#define VOXELRENDERER_HPP

#include <vector>

#include "cube/graphics/Vertex.hpp"
#include "cube/graphics/Shader.hpp"
#include "cube/graphics/Texture.hpp"
#include "cube/utils/ThreadPool.hpp"
#include "cube/world/Chunk.hpp"
#include "cube/world/World.hpp"

namespace cube {

    struct VoxelMesh {
        glm::ivec2 key;
        std::vector<Vertex3D> vertices;
        std::vector<uint32_t> indices;
    };

    struct VoxelItem {
        glm::uint vao;
        glm::uint vbo;
        glm::uint ebo;
        int count;

        void load();
        void upload(const std::vector<Vertex3D>& vertices, const std::vector<unsigned>& indices);
        void unload();
    };

    class VoxelRenderer {
    public:
        VoxelRenderer();
        ~VoxelRenderer();

        void onCreate();
        void onClear();
        void onResize(int w, int h);

        void onDraw(const glm::mat4&, const glm::vec3& root);
        void onTick(ThreadPool& pool, World& world);

        static VoxelMesh toMesh(const Chunk&);
    private:
        std::unordered_map<glm::ivec2, VoxelItem> m_items;
        std::mutex m_meshes_mutex;
        std::vector<VoxelMesh> m_meshes;
        glm::mat4 m_proj{1.f};
        Shader m_shader;
        Texture m_atlas;
    };

}

#endif //VOXELRENDERER_HPP
