#ifndef CUBE_ENGINE_HPP
#define CUBE_ENGINE_HPP
#include <memory>

#include "ScreenManager.hpp"

namespace cube {

    class Engine {
    public:
        Engine();
        ~Engine();

        void onInit();
        void onDeinit();
        void onUpdate(float);
        void onDraw();
        void onTick();
        void onEvent(const Event&);
    private:
        std::unique_ptr<ScreenManager> m_screen_manager;
    };

}

#endif //CUBE_ENGINE_HPP