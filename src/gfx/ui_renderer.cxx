#include "ui_renderer.hxx"

void UiRenderer::draw() {
}
/*
void UiRenderer::draw() {
    // !TODO: move into UI
    const auto& journal { hud_->journal_entries() };

    Vec2<u32> pos { 0, display_->dimensions().y - 1 };

    for (auto it { journal.crbegin() }; it != journal.crend(); ++it) {
        display_->text(*it, pos);

        if (--pos.y == 0) {
            break;
        }
    }
    // !
    draw_node(hud_->ui_tree().root());
}

void UiRenderer::draw_node(UiNode* node) {
    assert(node);
    if (!node->visible()) {
        return;
    }
    if (node->type() == UiNodeType::window) {
        auto window { static_cast<UiWindow*>(node) };
        const auto pos { window->absolute_position() };
        // border
        display_->rectangle({
            {
                pos.y,
                pos.x + window->size().x,
                pos.y + window->size().y,
                pos.x
            },
            Color::mono(128),
            Color::black(),
        });
        // title
        display_->text(window->title(), pos);
    }
    for (UiNode* child_node : node->children()) {
        draw_node(child_node);
    }
}
*/