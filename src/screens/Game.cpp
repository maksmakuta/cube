#include "cube/screens/Game.hpp"

#include <iostream>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "cube/utils/Utils.hpp"

namespace cube {
    
    Game::Game() : IScreen(){}
    Game::~Game() = default;

    void Game::onCreate() {}
    void Game::onClear(){}
    void Game::onDraw() {
    }

    void Game::onTick() {
        m_world.onTick(m_player.getPosition());
    }

    void Game::onUpdate(const float dt) {
        m_player.move(m_player_dir,dt);
    }

    void Game::onResize(int w, int h){}

    void Game::onKey(const int k, const int a, const int m) {
        const auto keys = std::vector{
            GLFW_KEY_W,
            GLFW_KEY_S,
            GLFW_KEY_A,
            GLFW_KEY_D,
            GLFW_KEY_SPACE,
            GLFW_KEY_LEFT_SHIFT
        };

        for(int i = 0; i < keys.size();i++) {
            if (k == keys[i]) {
                set(m_player_dir,1 << (i+1),a != GLFW_RELEASE);
            }
        }
    }

    void Game::onCursor(const float x, const float y) {
        m_player.rotate(x,y);
    }

    void Game::onScroll(float dx, float dy){}
    void Game::onText(uint) {}
    
}
