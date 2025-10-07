#ifndef CUBE_MAINSCREEN_HPP
#define CUBE_MAINSCREEN_HPP

#include "core/IScreen.hpp"

namespace cube {

    class MainScreen final : public IScreen{
    public:
        MainScreen();
        ~MainScreen() override;

        void onInit() override;
        void onDeinit() override;
        void onDraw(Context&) override;
        void onTick(float) override;
        void onEvent(const Event&) override;
    private:
        glm::vec2 mouse{};
        bool pressed{false};
        int join = 0;
        int cap = 0;
        int selected = -1;

        std::vector<glm::vec2> path;
    };

}

#endif //CUBE_MAINSCREEN_HPP