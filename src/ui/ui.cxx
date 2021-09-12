#include "ui.hxx"

Ui::Ui(entt::dispatcher& dispatcher) :
    dispatcher_ { dispatcher } {
    dispatcher_.sink<DisplayResizedEvent>().connect<&Ui::on_display_resized>(this);
    dispatcher_.sink<ScriptResetEvent>().connect<&Ui::on_script_reset>(this);
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

void Ui::on_display_resized(const DisplayResizedEvent& e) {
    if (auto root = ui_tree_.root()) {
        root->resize(e.size);
    }
}

void Ui::on_script_reset(const ScriptResetEvent& e) {
    ui_tree_.reset();
}

UiTree& Ui::ui_tree() {
    return ui_tree_;
}
