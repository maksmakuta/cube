#ifndef VOXELRENDERER_HPP
#define VOXELRENDERER_HPP

#include "cube/graphics/Shader.hpp"
#include "cube/world/Chunk.hpp"

namespace cube {

    class VoxelRenderer {
    public:
        VoxelRenderer();
        ~VoxelRenderer();

        void onCreate();
        void onClear();
        void onResize(int w, int h);

        void drawCube();
        void drawChunk(const Chunk&, const glm::vec2&);
    private:
        glm::mat4 m_projection{1.f};
        Shader m_shader;
        glm::uint m_vao{0}, m_vbo{0}, m_ebo{0};

    };

}

#endif //VOXELRENDERER_HPP
