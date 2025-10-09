#include "ScreenManager.hpp"

namespace cube {

    ScreenManager::ScreenManager() = default;
    ScreenManager::~ScreenManager() = default;

    std::shared_ptr<IScreen> ScreenManager::current() const {
        if (m_screens.empty()) {
            return nullptr;
        }
        return m_screens.back();
    }

    void ScreenManager::onInit(){
        if (current()) {
            current()->onInit();
        }
    }

    void ScreenManager::onDeinit(){
        if (current()) {
            current()->onDeinit();
        }
    }

    void ScreenManager::onDraw(Context& ctx){
        if (current()) {
            current()->onDraw(ctx);
        }
    }

    void ScreenManager::onTick(const float dt){
        if (current()) {
            current()->onTick(dt);
        }
    }

    void ScreenManager::onEvent(const Event& e){
        if (current()) {
            current()->onEvent(e);
        }
    }

    void ScreenManager::push(const std::shared_ptr<IScreen>& s){
        m_screens.push_back(s);
    }

    void ScreenManager::pop(){
        m_screens.pop_back();
    }

    void ScreenManager::clear(){
        m_screens.clear();
    }

}