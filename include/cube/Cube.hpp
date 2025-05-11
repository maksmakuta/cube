#ifndef CUBE_HPP
#define CUBE_HPP

#include "cube/core/ICallbacks.hpp"
#include "cube/entities/Player.hpp"
#include "cube/graphics/renderer/VoxelRenderer.hpp"
#include "cube/graphics/Mesh.hpp"

namespace cube {

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
        VoxelRenderer m_voxel;
        Chunk m_chunk;
        ChunkMesh m_chunk_mesh;
        Player m_player;
        uint8_t m_direction{0};
    };

}

#endif //CUBE_HPP
