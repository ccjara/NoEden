#include "ui.hxx"

Ui::Ui(EventManager& dispatcher) :
    events_ { dispatcher } {
    events_.on<DisplayResizedEvent>(this, &Ui::on_display_resized);
    events_.on<ScriptResetEvent>(this, &Ui::on_script_reset);
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

UiTree& Ui::ui_tree() {
    return ui_tree_;
}
