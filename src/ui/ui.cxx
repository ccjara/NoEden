#include "ui.hxx"

Ui::Ui(entt::dispatcher& dispatcher) :
    dispatcher_ { dispatcher },
    ui_proxy_ { &ui_tree_ } {
    dispatcher_.sink<Display_resized_event>().connect<&Ui::on_display_resized>(this);
    dispatcher_.sink<ScriptLoadedEvent>().connect<&Ui::on_script_loaded>(this);
    dispatcher_.sink<ScriptResetEvent>().connect<&Ui::on_script_reset>(this);
}

void Ui::update() {
    for (auto& [_, node_proxy] : ui_proxy_.node_proxies()) { // scripts
        node_proxy->call_handler();
    }
}

void Ui::on_script_loaded(const ScriptLoadedEvent& e) {
    e.script->declare<UiWindow_proxy>();

    e.script->define_global("ui", &ui_proxy_);

    luabridge::getGlobalNamespace(e.script->lua_state())
        .beginNamespace("anchor_origin")
            .addProperty("center", &lua_anchor_origin::center, false)
            .addProperty("top", &lua_anchor_origin::top, false)
            .addProperty("top_right", &lua_anchor_origin::top_right, false)
            .addProperty("right", &lua_anchor_origin::right, false)
            .addProperty("bottom_right", &lua_anchor_origin::bottom_right, false)
            .addProperty("bottom", &lua_anchor_origin::bottom, false)
            .addProperty("bottom_left", &lua_anchor_origin::bottom_left, false)
            .addProperty("left", &lua_anchor_origin::left, false)
            .addProperty("top_left", &lua_anchor_origin::top_left, false)
        .endNamespace();
}

void Ui::on_display_resized(const Display_resized_event& e) {
    ui_tree_.root()->resize(e.size);
}

void Ui::on_script_reset(const ScriptResetEvent& e) {
    ui_proxy_.reset();
    ui_tree_.reset();
}

UiTree& Ui::ui_tree() {
    return ui_tree_;
}
