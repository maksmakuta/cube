#ifndef CUBE_ISCREEN_HPP
#define CUBE_ISCREEN_HPP

#include <utility>

#include "graphics/Context.hpp"
#include "window/Event.hpp"

namespace cube {

    class IScreen;
    using ScreenPtr = std::shared_ptr<IScreen>;

    class ScreenController : public std::enable_shared_from_this<ScreenController>{
    public:
        virtual ~ScreenController() = default;

        virtual void navigate(const ScreenPtr&) = 0;
        virtual void navigateBack() = 0;

        template<class T, class... Args>
        void go(Args... args) {
            navigate(std::make_shared<T>(shared_from_this(), std::forward<Args>(args)...));
        }
    };

    using ControllerPtr = std::shared_ptr<ScreenController>;

    class IScreen {
    public:
        explicit IScreen(ControllerPtr controller) : m_controller(std::move(controller)) {}
        virtual ~IScreen() = default;

        virtual void onInit() = 0;
        virtual void onDeinit() = 0;
        virtual void onTick(float) = 0;
        virtual void onDraw(Context&) = 0;
        virtual void onEvent(const Event&) = 0;

        [[nodiscard]] ControllerPtr getController() const {
            return m_controller;
        }

    private:
        ControllerPtr m_controller;
    };

}

#endif //CUBE_ISCREEN_HPP