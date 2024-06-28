#include "ui/ui.hxx"
#include "ui_text.hxx"
#include "ui/ui_tree.hxx"
#include "ui/ui_window.hxx"
#include "gfx/display.hxx"
#include "gfx/renderer.hxx"

Ui::Ui(Renderer* renderer, EventManager* events) : events_(events), renderer_(renderer) {
    assert(events_);
    assert(renderer_);
}

bool Ui::initialize() {
    ui_tree_.create_root_node();

    display_resized_sub_ = events_->on<DisplayResizedEvent>(this, &Ui::on_display_resized);
    script_reset_sub_ = events_->on<ScriptResetEvent>(this, &Ui::on_script_reset);

    display_ = &renderer_->ui_layer();

    return true;
}

void Ui::update() {
    update_node(ui_tree_.root());
}

void Ui::draw() {
    draw_node(ui_tree_.root());
}

void Ui::update_node(UiNode* node) {
    if (node == nullptr) {
        return;
    }
    node->call_handler();

    for (UiNode* n : node->children()) {
        update_node(n);
    }
}

EventResult Ui::on_display_resized(const DisplayResizedEvent& e) {
    if (auto root = ui_tree_.root()) {
        root->resize(e.size);
    }
    return EventResult::Continue;
}

EventResult Ui::on_script_reset(const ScriptResetEvent&) {
    ui_tree_.reset();
    return EventResult::Continue;
}

UiTree& Ui::tree() {
    return ui_tree_;
}

void Ui::draw_node(UiNode* node) {
    assert(node);
    if (!node->visible()) {
        return;
    }

    if (node->type() == UiNodeType::window) {
        auto n = static_cast<UiWindow*>(node);
        const auto pos = n->absolute_position();
        // border
        display_->rectangle({
            Rect<i32> { pos, n->size() },
            Color::mono(128),
            Color::black(),
        });
        // title
        display_->text(n->title(), pos);
    } else if (node->type() == UiNodeType::text) {
        auto n = static_cast<UiText*>(node);
        display_->text(n->text(), n->absolute_position());
    }
    for (UiNode* child_node : node->children()) {
        draw_node(child_node);
    }
}
