#include "hud_system.hxx"

void j_hud_system::on_load() {
    define_task<j_gathering_completed_event, &j_hud_system::task_journal_item_pickup>();
    define_task<j_inventory_view_event, &j_hud_system::task_show_inventory_ui>();

    dispatcher_->sink<j_display_resized_event>().connect<&j_hud_system::immediate_on_display_resized>(this);
    dispatcher_->sink<j_script_loaded_event>().connect<&j_hud_system::immediate_on_script_loaded>(this);
    dispatcher_->sink<j_script_before_unload_event>().connect<&j_hud_system::immediate_on_script_before_unload>(this);
}

void j_hud_system::update(uint32_t delta_time) {
    queue_.update();

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

void j_hud_system::task_show_inventory_ui(const j_inventory_view_event& e) {
    if (!inventory_window_) {
        inventory_window_ = ui_.create_node<j_ui_window>(ui_.root(), "inventory_window");
        assert(inventory_window_);

        inventory_window_->set_title("Inventory");
        inventory_window_->set_anchor_origin(j_ui_anchor_origin::center);
        inventory_window_->resize({ 20, 10 });
        inventory_window_->move({ 0, 0 });
    }
}

j_ui_tree& j_hud_system::ui_tree() {
    return ui_;
}
