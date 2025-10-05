#ifndef CUBE_SCREENMANAGER_HPP
#define CUBE_SCREENMANAGER_HPP

#include <memory>
#include <vector>

#include "core/IScreen.hpp"
#include "window/Event.hpp"

namespace cube {

    class ScreenManager final : public IScreen{
    public:
        ScreenManager();
        ~ScreenManager() override;

        [[nodiscard]] std::shared_ptr<IScreen> current() const;

        void onInit() override;
        void onDeinit() override;
        void onDraw(Context&) override;
        void onTick(float) override;
        void onEvent(const Event &) override;

        void push(const std::shared_ptr<IScreen> &);
        void pop();
        void clear();
    private:
        std::vector<std::shared_ptr<IScreen>> m_screens;
    };

}

#endif //CUBE_SCREENMANAGER_HPP