#ifndef CUBE_HPP
#define CUBE_HPP

#include "cube/base/game.hpp"
#include "cube/graphics/shader.hpp"
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
    protected:
        static int getDir(int key);

    private:
        render::Camera m_camera;
        glm::mat4 m_proj{1.f};
        glm::vec2 m_last{0};
        uint8_t m_direction{0};

        graphics::Shader m_shader;
        uint VAO{0}, VBO{0};
    };

}

#endif //CUBE_HPP
