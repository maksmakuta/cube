#include "MainScreen.hpp"

#include <iostream>

#include "utils/LambdaVisitor.hpp"

namespace cube {

    MainScreen::MainScreen() = default;
    MainScreen::~MainScreen() = default;

    void MainScreen::onInit() {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void MainScreen::onDeinit() {

    }

    void MainScreen::onDraw(Context& ctx) {
        clear(Color(0xFF202020));

        const auto ctx2d = ctx.getRenderer2D();
        if (!ctx2d) {
            std::cout << "No 2D renderer found" << std::endl;
            return;
        }

        ctx2d->begin();
        ctx2d->setJoin(static_cast<JoinType>(join));
        ctx2d->setCap(static_cast<CapType>(cap));
        ctx2d->stroke(Color(0xFF0080FF), 50.f, false);
        ctx2d->path(path);
        for (const auto& v : path) {
            ctx2d->fill(Color(0xFFFF8000));
            ctx2d->circle(v, 10.f);
        }
        ctx2d->end();
    }

    void MainScreen::onTick(float) {

    }

    void MainScreen::onEvent(const Event& e) {
        std::visit(LambdaVisitor{
            [](const ResizeEvent& re) {},
            [this](const KeyEvent& ke) {
                pressed = ke.pressed && ke.key == Key::MouseLeft;
                if (ke.pressed) {
                    if (ke.key == Key::MouseRight) {
                        path.emplace_back(mouse);
                    }
                    if (ke.key == Key::Space) {
                        path.clear();
                    }

                    if (ke.key == Key::W) {
                        join++;
                        join = std::min(join, 3);
                    }
                    if (ke.key == Key::S) {
                        join--;
                        join = std::max(join, 0);
                    }

                    if (ke.key == Key::D) {
                        cap++;
                        cap = std::min(cap, 3);
                    }
                    if (ke.key == Key::A) {
                        cap--;
                        cap = std::max(cap, 0);
                    }
                }
            },
            [this](const MouseEvent& me) {
                mouse = {me.x,me.y};
                if (pressed) {
                    selected = -1;
                    for (auto i = 0; i < path.size(); i++) {
                        const auto& v = path[i];
                        if (glm::distance(mouse, v) <= 20.f) {
                            selected = i;
                        }
                    }
                    if (selected != -1) {
                        path[selected] = mouse;
                    }
                }
            },
            [](const ScrollEvent& se) {},
            [](const InputEvent& ie) {}
        }, e);
    }

}
