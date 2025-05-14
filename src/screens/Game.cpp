#include "cube/screens/Game.hpp"

namespace cube {
    
    Game::Game() : IScreen(){}
    Game::~Game() = default;

    void Game::onCreate(){}
    void Game::onClear(){}
    void Game::onDraw(){}
    void Game::onTick(){}

    void Game::onUpdate(float dt){}
    void Game::onResize(int w, int h){}
    void Game::onKey(int k, int a, int m){}
    void Game::onCursor(float x, float y){}
    void Game::onScroll(float dx, float dy){}
    
}