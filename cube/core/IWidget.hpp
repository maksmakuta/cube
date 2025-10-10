#ifndef CUBE_IWIDGET_HPP
#define CUBE_IWIDGET_HPP

#include "graphics/RendererUI.hpp"
#include "window/Event.hpp"

namespace cube {

    class IWidget;

    using WidgetPtr = std::shared_ptr<IWidget>;

    class IWidget {
    public:
        explicit IWidget(const WidgetPtr& parent = nullptr) {
            setParent(parent);
        }
        virtual ~IWidget() = default;

        virtual void onTick(float) = 0;
        virtual void onDraw(RendererUI&) = 0;
        virtual void onEvent(const Event&) = 0;

        void setPos(const glm::vec2& p) {
            m_pos = p;
        }

        void setSize(const glm::vec2& s) {
            m_size = s;
        }

        [[nodiscard]] glm::vec2 getPos() const {
            return m_pos;
        }

        [[nodiscard]] glm::vec2 getSize() const {
            return m_size;
        }

        void setVisible(const bool v) { m_visible = v; }
        void setEnabled(const bool e) { m_enabled = e; }

        [[nodiscard]] bool isVisible() const { return m_visible; }
        [[nodiscard]] bool isEnabled() const { return m_enabled; }


        void setParent(const WidgetPtr& p) { m_parent = p; }
        [[nodiscard]] WidgetPtr getParent() const { return m_parent; }

    protected:
        glm::vec2 m_pos{};
        glm::vec2 m_size{};
        bool m_visible = true;
        bool m_enabled = true;
        WidgetPtr m_parent = nullptr;
    };

}

#endif //CUBE_IWIDGET_HPP