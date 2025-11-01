#include "Cube.h"
#include "utils/LambdaVisitor.hpp"
#include "utils/AssetsPaths.hpp"

namespace cube{

    Cube::Cube(){
        m_renderer = std::make_unique<Renderer2D>();
    }

    void Cube::onInit(){
        m_font.load(getFont("Monocraft-Regular.ttf"),24);
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
        ctx.text(m_font,Color(0xFFFFFFFF));
        ctx.print("Hello, World", mouse);
    }

}
