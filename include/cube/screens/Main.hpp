#ifndef MAIN_HPP
#define MAIN_HPP

#include "cube/core/IScreen.hpp"
#include "../graphics/Renderer.hpp"

namespace cube {

    class Main final : public  IScreen {
    public:
        Main();
        ~Main() override;

        void onCreate() override;
        void onClear() override;
        void onDraw() override;
        void onTick() override;

        void onUpdate(float dt) override;
        void onResize(int w, int h) override;
        void onKey(int k, int a, int m) override;
        void onCursor(float x, float y) override;
        void onScroll(float dx, float dy) override;
        void onText(uint) override;
    private:
        Renderer m_renderer;
        Font m_font{32};
        glm::vec2 m{0};
    };
}

#endif //MAIN_HPP
