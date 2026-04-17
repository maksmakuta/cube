#ifndef CUBE_WINDOW_HPP
#define CUBE_WINDOW_HPP

#include <string>
#include <SDL3/SDL_video.h>
#include <glm/vec2.hpp>

namespace cube {

    class Window final {
    public:
        explicit Window(const std::string& name, const glm::ivec2& size = {800, 600});
        ~Window();

        void setTitle(const std::string& title) const;
        void setSize(const glm::ivec2& size) const;
        void setPosition(const glm::ivec2& position) const;

        [[nodiscard]] std::string getTitle() const;
        [[nodiscard]] glm::ivec2 getSize() const;
        [[nodiscard]] glm::ivec2 getPosition() const;

        void close();
        void swapBuffers() const;

        [[nodiscard]] bool isOpen() const;

    private:
        bool m_is_open = false;
        SDL_Window* m_window = nullptr;
        SDL_GLContext m_context = nullptr;
    };

}

#endif //CUBE_WINDOW_HPP
