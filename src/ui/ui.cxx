#include "ui.hxx"

Ui::Ui(EventManager& dispatcher, Display& display) :
    events_ { dispatcher },
    display_ { display } {
    events_.on<DisplayResizedEvent>(this, &Ui::on_display_resized);
    events_.on<ScriptResetEvent>(this, &Ui::on_script_reset);
    events_.on<PostWorldRenderEvent>(this, &Ui::on_post_world_render);
}

void Ui::update() {
    for (auto& node : ui_tree_.nodes()) {
        node->call_handler();
    }
}

void Ui::startup() {
    ui_tree_.create_root_node();
}

void Ui::shutdown() {
    ui_tree_.clear();
}

bool Ui::on_display_resized(DisplayResizedEvent& e) {
    if (auto root = ui_tree_.root()) {
        root->resize(e.size);
    }
    return false;
}

bool Ui::on_script_reset(ScriptResetEvent& e) {
    ui_tree_.reset();
    return false;
}

bool Ui::on_post_world_render(PostWorldRenderEvent& e) {
    draw_node(ui_tree_.root());
    return false;
}

UiTree& Ui::ui_tree() {
    return ui_tree_;
}

void Ui::draw_node(UiNode* node) {
    assert(node);
    if (!node->visible()) {
        return;
    }

    if (node->type() == UiNodeType::window) {
        auto window { static_cast<UiWindow*>(node) };
        const auto pos { window->absolute_position() };
        // border
        display_.rectangle({
            Rect<i32> { pos, window->size() },
            Color::mono(128),
            Color::black(),
        });
        // title
        display_.text(window->title(), pos);
    } else if (node->type() == UiNodeType::text) {
        auto text_node { static_cast<UiText*>(node) };
        display_.text(
            text_node->text(),
            text_node->absolute_position()
        );
    }
    for (UiNode* child_node : node->children()) {
        draw_node(child_node);
    }
}
