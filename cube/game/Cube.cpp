#include "Cube.hpp"

#include <format>
#include <random>

#include "graphics/Color.hpp"
#include "utils/AssetsPaths.hpp"
#include "utils/LambdaVisitor.hpp"

namespace cube {

    constexpr auto CHUNK_DISTANCE = 16;

    constexpr glm::ivec2 DIRECTIONS[4] = {
        {1,0},
        {-1,0},
        {0,1},
        {0,-1},
    };

    int randomSeed() {
        return static_cast<int>(std::mt19937{std::random_device{}()}());
    }

    Cube::Cube(const std::shared_ptr<Window>& window) :
        m_renderer(std::make_unique<ChunkRenderer>()),
        m_2d_renderer(std::make_unique<Renderer2D>()),
        m_window(window)
    {
        m_font.load(getFont("Monocraft-Regular.ttf"),24);
        //
        // const auto pos = glm::ivec2{0,0};
        // auto chunk = m_gen.generateChunk(pos);
        // m_world.setChunk(pos,chunk);
        // const auto mesh = toMesh(chunk, std::vector<ChunkPtr>{4,nullptr});
        // m_renderer->add(pos,mesh);
    }

    Cube::~Cube() = default;

    void Cube::onDraw() const {
        clear(0xFF141852);
        m_renderer->draw(m_camera.getMatrix());
        if (is_debug)
            onDrawUI(*m_2d_renderer);
    }

    void Cube::onDrawUI(const Renderer2D& ctx) const {
        ctx.begin();
        ctx.text(m_font, Color(0xFFFFFFFF));
        ctx.print(std::format("FPS: {}", m_fps), {0, m_font.getSize()});
        ctx.end();
    }

    std::vector<ChunkPtr> getNeighbors(World& world, const glm::ivec2& pos) {
        auto result = std::vector<ChunkPtr>(4);
        for (auto i = 0; i < 4; i++) {
            result[i] = world.getChunk(pos + DIRECTIONS[i]);
        }
        return result;
    }

    void Cube::onUpdate(const float dt) {
        m_camera.update(dt);
        m_fps = 1.0f / dt;


    }

    void Cube::onEvent(const Event& e) {
        std::visit(LambdaVisitor{
            [this](const ResizeEvent& re) {
                m_renderer->resize({re.width, re.height},m_camera.getFOV());
                m_2d_renderer->resize({re.width, re.height});
            },
            [](const MouseEvent&) {},
            [this](const KeyEvent& ke) {
                if (ke.pressed && ke.key == Key::F1) {
                    is_debug = !is_debug;
                }
            },
            [](const ScrollEvent&) {},
            [](const InputEvent&) {},
        }, e);
        m_camera.onEvent(e);
    }

}
