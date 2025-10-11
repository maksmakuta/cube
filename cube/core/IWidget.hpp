#ifndef CUBE_IWIDGET_HPP
#define CUBE_IWIDGET_HPP

#include "graphics/RendererUI.hpp"
#include "ui/LayoutParams.hpp"
#include "ui/Rect.hpp"
#include "window/Event.hpp"

namespace cube {

    class IWidget;

    using WidgetPtr = std::shared_ptr<IWidget>;

    class IWidget : public std::enable_shared_from_this<IWidget>{
    public:
        explicit IWidget(const WidgetPtr& parent = nullptr) {
            setParent(parent);
        }
        virtual ~IWidget() = default;

        virtual void onTick(float) = 0;
        virtual void onDraw(RendererUI&) = 0;
        virtual void onEvent(const Event&) = 0;

        [[nodiscard]] WidgetPtr getParent() const { return m_parent; }
        [[nodiscard]] const LayoutParams& getLayoutParams() const { return m_params; }
        [[nodiscard]] Rect& getRect() { return m_rect; }

        void setParent(const WidgetPtr& p) { m_parent = p; }
        void setLayoutParams(const LayoutParams& p) { m_params = p; }
        void setRect(const Rect& r) { m_rect = r; }

    protected:
        Rect m_rect;
        LayoutParams m_params{};
        WidgetPtr m_parent = nullptr;
    };

}

#endif //CUBE_IWIDGET_HPP