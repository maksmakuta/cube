#include "Cube.h"
#include "utils/LambdaVisitor.hpp"
#include "utils/Assets.hpp"

#include <format>

namespace cube{

    Cube::Cube(){
        m_renderer = std::make_unique<Renderer2D>();
    }

    void Cube::onInit(){
        m_font.load(Assets::getFont(Italic),24);
    }

    void Cube::onDeinit(){
        m_font.unload();
    }

    void Cube::onDraw(){
        clear(0xFFA0A0A0);
        m_renderer->begin();
        onRender(*m_renderer);
        m_renderer->end();
    }

    void Cube::onTick(float dt){
        m_fps = 1.0f / dt;
    }

    void Cube::onEvent(const Event& e){
        std::visit(LambdaVisitor{
            [this](const ResizeEvent& re){
                m_renderer->resize({re.width, re.height});
            },
            [](const KeyEvent&){},
            [this](const MouseEvent& me){
                mouse = {me.x, me.y};
            },
            [](const ScrollEvent&){},
            [](const InputEvent&){},
        },e);
    }

    void Cube::onRender(Renderer2D& ctx){
        const auto line = m_font.getSize();
        ctx.text(m_font,Color(0xFFFFFFFF));
        ctx.print("Hello, World", {0,line});
        ctx.print(std::format("FPS: {}",m_fps), {0,line*2});
    }

}
