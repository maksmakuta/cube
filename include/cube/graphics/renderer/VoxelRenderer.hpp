#ifndef VOXELRENDERER_HPP
#define VOXELRENDERER_HPP

#include <vector>

#include "Vertex.hpp"
#include "cube/graphics/Shader.hpp"
#include "cube/graphics/Texture.hpp"

namespace cube {

    class VoxelRenderer {
    public:
        VoxelRenderer();
        ~VoxelRenderer();

        void onCreate();
        void onClear();
        void onResize(int w, int h);

        void draw(const std::vector<Vertex3D>& mesh, const glm::vec2& pos, const glm::mat4& view) const;

        void setLigth(const glm::vec3& vec);


    private:
        glm::mat4 m_projection{1.f};
        Shader m_shader;
        Texture m_atlas;
        glm::uint m_vao{0}, m_vbo{0};

    };

}

#endif //VOXELRENDERER_HPP
