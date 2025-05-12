#ifndef CUBE_HPP
#define CUBE_HPP

#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "cube/core/ICallbacks.hpp"
#include "cube/entities/Player.hpp"
#include "cube/graphics/renderer/VoxelRenderer.hpp"
#include "cube/world/World.hpp"
#include "graphics/Mesh.hpp"

namespace cube {

    struct FiniteWorld {
        std::unordered_map<glm::ivec2, std::shared_ptr<Chunk>> chunks;
        std::unordered_map<glm::ivec2, ChunkMesh> meshes;
    };

    class Cube final : public ICallbacks{
    public:
        Cube();

        void onCreate();
        void onClear();
        void onDraw();

        void onUpdate(float dt) override;
        void onResize(int w, int h) override;
        void onKey(int k, int a, int m) override;
        void onCursor(float x, float y) override;
        void onScroll(float dx, float dy) override;

    private:
        FiniteWorld m_world{};
        VoxelRenderer m_voxel;
        Player m_player;
        float last_tick = 0.0;
        uint8_t m_direction{0};
    };

}

#endif //CUBE_HPP
