#ifndef CUBE_CONTEXT_HPP
#define CUBE_CONTEXT_HPP

#include <memory>

#include "Renderer2D.hpp"

namespace cube {

    class Context {
    public:
        Context() = default;
        ~Context() = default;

        void setRenderer(const std::shared_ptr<Renderer2D>& renderer);
        [[nodiscard]] std::shared_ptr<Renderer2D> getRenderer2D() const;
    private:
        std::shared_ptr<Renderer2D> m_2d_renderer;
    };

    void clear(const Color&);

}

#endif //CUBE_CONTEXT_HPP