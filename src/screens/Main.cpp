#include "cube/screens/Main.hpp"

namespace cube {

    const auto text = "Hello, Cube";

    Main::Main() : IScreen(){}
    Main::~Main() = default;

    void Main::onCreate() {
        m_renderer.onCreate();
        m_font.load(getAsset("/fonts/BlockCraft.otf"));
    }

    void Main::onClear() {
        m_renderer.onClear();
        m_font.unload();
    }

    void Main::onDraw() {
        clear(0xFF222222);

        m_renderer.text(m_font,0xFFFFFFFF);
        m_renderer.print(view/2.f,text, Align::Center);

        m_renderer.fill(0xFFFF0000);
        m_renderer.rect(view.x/2 - m_font.width(text)/2,view.y/2,m_font.width(text),-m_font.height(text));
    }

    void Main::onTick() {

    }

    void Main::onUpdate(float dt){}

    void Main::onResize(const int w, const int h) {
        view = {w,h};
        m_renderer.onResize(w,h);
    }

    void Main::onKey(int k, int a, int m){}

    void Main::onCursor(float x, float y) {
        mouse = {x,y};
    }

    void Main::onScroll(float dx, float dy){}
    void Main::onText(uint) {

    }

}