#include "Button.hpp"

#include <utility>

namespace cube {

    Button::Button(LayoutPtr  view) : m_view(std::move(view)){}
    Button::~Button() = default;

    void Button::onTick(float) {

    }

    void Button::onDraw(RendererUI&) {

    }

    void Button::onEvent(const Event&) {

    }

}