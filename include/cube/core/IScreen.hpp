#ifndef ISCREEN_HPP
#define ISCREEN_HPP
#include "IController.hpp"

namespace cube {

    using uint = unsigned int;

    class IScreen{
    public:
        virtual ~IScreen() = default;

        virtual void onCreate() = 0;
        virtual void onClear() = 0;
        virtual void onDraw() = 0;
        virtual void onTick() = 0;
        virtual void onUpdate(float dt) = 0;
        virtual void onResize(int w, int h) = 0;
        virtual void onKey(int k, int a, int m) = 0;
        virtual void onCursor(float x, float y) = 0;
        virtual void onScroll(float dx, float dy) = 0;
        virtual void onText(uint) = 0;

        void attach(IController* c) {
            m_controller = c;
        }

        [[nodiscard]] IController* controller() const {
            return m_controller;
        }
    protected:
        IController* m_controller = nullptr;

    };

}

#endif //ISCREEN_HPP
