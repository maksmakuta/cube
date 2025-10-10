#ifndef CUBE_ILAYOUT_HPP
#define CUBE_ILAYOUT_HPP

#include <algorithm>

#include "IWidget.hpp"
#include "ui/LayoutParams.hpp"
#include "ui/Rect.hpp"

namespace cube {

    class ILayout : public IWidget {
    public:
        ILayout() = default;
        ~ILayout() override = default;

        virtual glm::vec2 onMeasure(const glm::vec2& size) = 0;
        virtual void onLayout(const Rect& container) = 0;

        void addChild(const WidgetPtr& w, const LayoutParams& p) {
            if (w) {
                w->setLayoutParams(p);
                m_children.push_back(w);
            }
        }

        void removeChild(const WidgetPtr& widget) {
            const auto it = std::ranges::remove_if(m_children, [widget](const WidgetPtr& w) {
                return widget == w;
            });
            m_children.erase(it.begin(), m_children.end());
        }

        void onTick(const float t) override {
            for (const auto& child : m_children) {
                child->onTick(t);
            }
        }

        void onDraw(RendererUI& r) override {
            for (const auto& child : m_children) {
                child->onDraw(r);
            }
        }

        void onEvent(const Event& e) override {
            for (const auto& child : m_children) {
                child->onEvent(e);
            }
        }

    protected:
        std::vector<WidgetPtr> m_children;
    };

}

#endif //CUBE_ILAYOUT_HPP
