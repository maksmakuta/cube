#include "cube/Cube.hpp"
#include "cube/utils/AssetsUtils.hpp"

namespace cube {

    Cube::Cube() = default;

    void Cube::onCreate() {
        m_renderer.onCreate();
        m_voxel.onCreate();
        atlas.load(getAsset("/textures/atlas.png"));
        atlas.setWrap(TextureWrap::Repeat);
        atlas.setMagFilter(TextureFilter::Nearest);
        atlas.setMinFilter(TextureFilter::Nearest);
    }

    void Cube::onClear() {
        m_renderer.onClear();
        m_voxel.onClear();
    }

    void Cube::onDraw() {
        clear(0xFF202020);
        m_renderer.fill(0xFF00FF00);
        m_renderer.rect(mouse.x,mouse.y, 100,100);

        m_renderer.stroke(0xFF0000FF,5, LineCap::Loop);
        m_renderer.rect(mouse.x - 100,mouse.y - 100, 100,100);

        m_renderer.image(atlas, {0,0},{1.f / 10.f, 2.f/ 32.f});
        m_renderer.rect(0,0,256,256);
    }

    void Cube::onUpdate(float dt) {

    }

    void Cube::onResize(const int w, const int h){
        m_renderer.onResize(w,h);
    }

    void Cube::onKey(int k, int a, int m){

    }

    void Cube::onCursor(float x, float y){
        mouse = {x,y};
    }

    void Cube::onScroll(float dx, float dy){

    }
}

