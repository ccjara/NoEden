#include "hud_system.hxx"

void j_hud_system::on_load() {
    events_->bind<j_gathering_completed_event, &j_hud_system::task_journal_item_pickup>(this);
    events_->bind<j_display_resized_event, &j_hud_system::immediate_on_display_resized>(
        this,
        queue_consume_immediate_tag{}
    );
    events_->bind<j_script_loaded_event, &j_hud_system::immediate_on_script_loaded>(
        this,
        queue_consume_immediate_tag{}
    );
    events_->bind<j_script_before_unload_event, &j_hud_system::immediate_on_script_before_unload>(
        this,
        queue_consume_immediate_tag{}
    );

    ui_.root()->show();
}

void j_hud_system::update(uint32_t delta_time) {
    events_->process();

    for (auto& [_, node_proxy] : ui_proxy_.node_proxies()) { // scripts
        node_proxy->call_handler();
    }
}

void j_hud_system::immediate_on_script_loaded(const j_script_loaded_event& e) {
    e.script->declare<j_ui_window_proxy>();
    e.script->define_global("ui", &ui_proxy_);
}

void j_hud_system::immediate_on_script_before_unload(const j_script_before_unload_event& e) {
    ui_proxy_.clear_dependencies_by_state(e.script->lua_state());
}

void j_hud_system::immediate_on_display_resized(const j_display_resized_event& e) {
    ui_.root()->resize(e.size);
}

const std::vector<std::string>& j_hud_system::journal_entries() const {
    return journal_.entries();
}

void j_hud_system::task_journal_item_pickup(const j_gathering_completed_event& e) {
    std::string log_str { "Picked up " };

    if (game->entities()->has<jc_object_descriptor>(e.item)) {
        log_str += "$c00ff00";
        log_str += game->entities()->get<jc_object_descriptor>(e.item).label;
        log_str += "$!";
    }
    else {
        log_str += " something";
    }

    journal_.emplace_back(std::move(log_str));
}

j_ui_tree& j_hud_system::ui_tree() {
    return ui_;
}
