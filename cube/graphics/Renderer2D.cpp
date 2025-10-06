#include "Renderer2D.hpp"

namespace cube {
    Renderer2D::Renderer2D() {

    }

    Renderer2D::~Renderer2D() {

    }

    void Renderer2D::resize(const glm::ivec2&) {

    }

    void Renderer2D::begin() {

    }

    void Renderer2D::end() {

    }

    void Renderer2D::flush() {

    }

    void Renderer2D::point(const glm::vec2&){

    }

    void Renderer2D::line(const glm::vec2& a, const glm::vec2& b){

    }

    void Renderer2D::triangle(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c){

    }

    void Renderer2D::quad(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d){

    }

    void Renderer2D::rect(const glm::vec2& origin, const glm::vec2& size){

    }

    void Renderer2D::rect(const glm::vec2& origin, const glm::vec2& size, float r){

    }

    void Renderer2D::rect(const glm::vec2& origin, const glm::vec2& size, const glm::vec2& r){

    }

    void Renderer2D::rect(const glm::vec2& origin, const glm::vec2& size, const glm::vec4& r){

    }

    void Renderer2D::circle(const glm::vec2& center, float r){

    }

    void Renderer2D::ellipse(const glm::vec2& center, const glm::vec2& r){

    }

    void Renderer2D::arc(const glm::vec2& center, const glm::vec2& r, const glm::vec2& range){

    }

    void Renderer2D::pie(const glm::vec2& center, const glm::vec2& r, const glm::vec2& range){

    }

    void Renderer2D::fill(const Color&){

    }

    void Renderer2D::fill(const Texture&){

    }

    void Renderer2D::stroke(const Color&, float w){

    }

}