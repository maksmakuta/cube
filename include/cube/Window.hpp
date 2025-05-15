#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include "core/IWindowController.hpp"
#include "cube/Cube.hpp"

namespace cube {

    struct WindowParams {
        uint8_t gl_major{3};
        uint8_t gl_minor{3};
        uint8_t gl_samples{4};
        uint8_t gl_depth{32};

        bool is_core{true};
    };

    class Window final : public IWindowController{
    public:
        explicit Window(WindowParams = {});
        ~Window() override;

        void attach(const std::shared_ptr<Cube>&);
        void run();

        [[nodiscard]] bool isClose() const;

        void close() override;
        void showCursor(bool) override;
        void setCursor(CursorIcon) override;

    private:
        std::shared_ptr<Cube> m_game;
        void* m_window = nullptr;
    };

}

#endif //WINDOW_HPP
