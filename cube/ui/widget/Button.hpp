#ifndef CUBE_BUTTON_HPP
#define CUBE_BUTTON_HPP

#include "core/ILayout.hpp"
#include "core/IWidget.hpp"

namespace cube {

    class Button final : public IWidget{
    public:
        explicit Button(LayoutPtr  view);
        ~Button() override;

        void onTick(float) override;
        void onDraw(RendererUI&) override;
        void onEvent(const Event&) override;
    private:
        LayoutPtr m_view;
    };

}

#endif //CUBE_BUTTON_HPP