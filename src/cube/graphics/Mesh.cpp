#include "cube/graphics/Mesh.hpp"

namespace cube {

    void mesh(const glm::ivec3& pos, const World& world, Renderer& renderer, ThreadPool& threadPool) {
        auto chunk = world.getChunkReadonly(pos);
        if (!chunk) return;


        //renderer.submit(pos, std::move(payload));
    }

}