#ifndef CUBE_CONTEXT_HPP
#define CUBE_CONTEXT_HPP

#include <memory>

#include "Renderer2D.hpp"
#include "Renderer3D.hpp"

namespace cube {

    class Context {
    public:
        Context() = default;
        ~Context() = default;

        void setRenderer(const std::shared_ptr<Renderer2D>&);
        void setRenderer(const std::shared_ptr<Renderer3D>&);

        [[nodiscard]] std::shared_ptr<Renderer2D> get2DRenderer() const;
        [[nodiscard]] std::shared_ptr<Renderer3D> get3DRenderer() const;

    private:
        std::shared_ptr<Renderer2D> m_2d_renderer = nullptr;
        std::shared_ptr<Renderer3D> m_3d_renderer = nullptr;
    };

    void clear(const Color&);

}

#endif //CUBE_CONTEXT_HPP