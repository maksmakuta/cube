#include "cube/Cube.hpp"

#include <functional>
#include <iostream>

#include "cube/core/Constants.hpp"
#include "cube/screens/Game.hpp"
#include "glad/gl.h"

namespace cube {

    Cube::Cube() = default;

    void Cube::onCreate() {
        m_screen = std::make_unique<Game>();
        m_screen->attach(this);
        m_screen->onCreate();
    }

    void Cube::onClear() {
        m_screen->onClear();
    }

    void Cube::onDraw() {
        m_screen->onDraw();
    }

    void Cube::onUpdate(const float dt) {
        m_screen->onUpdate(dt);
        lastTick += dt;
        if (lastTick > TICK) {
            m_screen->onTick();
            lastTick = 0;
        }
    }

    void Cube::onResize(const int w, const int h){
        glViewport(0,0,w,h);
        m_screen->onResize(w,h);
    }

    void Cube::onKey(const int k, const int a, const int m){
        m_screen->onKey(k,a,m);
    }

    void Cube::onCursor(const float x, const float y){
        m_screen->onCursor(x,y);
    }

    void Cube::onScroll(const float dx, const float dy){
        m_screen->onScroll(dx,dy);
    }

    void Cube::onText(uint code) {
        m_screen->onText(code);
    }

    void onController(IWindowController* c,const std::function<void(IWindowController*)>& fn) {
        if (c){
            fn(c);
        }else {
            std::cerr << "No window controllers attached!" << std::endl;
        }
    }

    void Cube::showCursor(bool b) {
        onController(m_win_controller,[&b](IWindowController* c){
            c->showCursor(b);
        });
    }

    void Cube::setCursor(CursorIcon i) {
        onController(m_win_controller,[&i](IWindowController* c){
            c->setCursor(i);
        });
    }

    void Cube::navigate(IScreen* ptr) {
        m_screen->onClear();
        m_screen.reset(ptr);
        if (m_screen) {
            m_screen->attach(this);
            m_screen->onCreate();
        }
    }

    void Cube::close() {
        onController(m_win_controller,[](IWindowController* c){
            c->close();
        });
    }

    void Cube::attachController(IWindowController* c) {
        m_win_controller = c;
    }

}

