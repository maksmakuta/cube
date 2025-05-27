#ifndef VOXELRENDERER_HPP
#define VOXELRENDERER_HPP

#include <shared_mutex>
#include <unordered_map>

#include "cube/graphics/Mesher.hpp"
#include "cube/graphics/Shader.hpp"
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

        void onTick(ThreadPool &, World &);

        void onResize(int w, int h);

        int count() const;

    private:
        mutable std::shared_mutex m_qmutex;
        glm::mat4 m_proj{1.f};
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
