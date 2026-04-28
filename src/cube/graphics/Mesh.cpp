#include "cube/graphics/Mesh.hpp"

namespace cube {

    void mesh(const glm::ivec3& pos, const World& world, Renderer& renderer, ThreadPool& threadPool) {
        //TODO(Meshing algorithm)
        // auto chunk = world.getChunkReadonly(pos);
        // if (!chunk) return;

        // auto right = world.getChunkReadonly(pos + glm::ivec3(1, 0, 0));
        // auto left  = world.getChunkReadonly(pos + glm::ivec3(-1, 0, 0));

        //renderer.submitMesh(pos, std::move(payload));
    }

}