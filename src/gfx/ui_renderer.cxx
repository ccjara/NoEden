#include "ui_renderer.hxx"

void j_ui_renderer::draw() {
    // !TODO: move into UI
    const auto& journal { hud_->journal_entries() };

    j_vec2<uint32_t> pos { 0, display_->dimensions().y - 1 };

    for (auto it { journal.crbegin() }; it != journal.crend(); ++it) {
        display_->text(*it, pos);

        if (--pos.y == 0) {
            break;
        }
    }
    // !
    draw_node(hud_->ui_tree().root());
}

void j_ui_renderer::draw_node(j_ui_node* node) {
    assert(node);
    if (node->node_type == j_ui_node_type::window) {
        auto window { static_cast<j_ui_window*>(node) };
        // border
        display_->rectangle({
            {
                window->position.y,
                window->position.x + window->size.x,
                window->position.y + window->size.y,
                window->position.x
            },
            j_color::mono(128),
            j_color::black(),
        });
        // title
        display_->text(window->title, window->position);
    }
    for (j_ui_node* child_node : node->children) {
        draw_node(child_node);
    }
}
