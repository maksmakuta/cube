#ifndef VOXELITEM_HPP
#define VOXELITEM_HPP

#include <vector>

#include "cube/graphics/Vertex.hpp"

namespace cube {

    struct VoxelRenderable {
        glm::uint vao;
        glm::uint vbo;
        glm::uint ebo;
        int count;

        void load();
        void upload(const std::vector<Vertex3D>& vertices, const std::vector<unsigned>& indices);
        void unload();
    };

    struct VoxelItem {
        VoxelRenderable terrain;
        VoxelRenderable billboard;
    };

}

#endif //VOXELITEM_HPP
