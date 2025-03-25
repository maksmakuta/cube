#ifndef CUBE_HPP
#define CUBE_HPP

#include "cube/base/game.hpp"
#include "cube/render/camera.hpp"

namespace cube::game {

    class Cube final : public IGame {
    public:
        Cube() = default;
        void init() override;
        void clear() override;
        void draw() override;
        void update(float delta) override;

        void onResize(int w, int h) override;
        void onCursor(float x, float y) override;
        void onKey(int key, int action, int mods) override;
    private:
        render::Camera m_camera;

        glm::vec2 last{0};
        uint8_t direction{0};
    };

}

#endif //CUBE_HPP
