#ifndef VOXELRENDERER_HPP
#define VOXELRENDERER_HPP
#include <unordered_map>

#include "Mesher.hpp"
#include "Shader.hpp"
#include "cube/utils/ThreadPool.hpp"
#include "cube/world/World.hpp"

namespace cube {
    struct DrawElementsIndirectCommand {
        uint count;
        uint instanceCount;
        uint firstIndex;
        uint baseVertex;
        uint baseInstance;
    };

    class VoxelRenderer {
    public:
        VoxelRenderer();

        ~VoxelRenderer();

        void onCreate();

        void onClear();

        void onDraw(const glm::mat4 &view);

        void onTick(ThreadPool &, const World &);

        void onResize(int w, int h);

    private:
        void load();

        glm::mat4 m_proj;
        std::unordered_map<glm::ivec2, Mesh> m_mesh_cache;
        Shader m_shader;
        uint commandCount{0};
        uint indirectBuffer{0};
        uint instanceBuffer{0};
        uint vao{0};
        uint vbo{0};
        uint ebo{0};
    };
}

#endif //VOXELRENDERER_HPP
