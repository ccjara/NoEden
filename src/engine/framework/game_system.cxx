#include "game_system.hxx"

std::vector<std::unique_ptr<task>>& game_system::submit_tasks() {
    return tasks_;
};

void game_system::attach_platform(entt::dispatcher& dispatcher) {
}

void game_system::attach(change_controller& controller) {
}

void game_system::on_load() {
}

void game_system::on_systems_loaded() {
}

const system_scene& game_system::scene() const noexcept {
    return *scene_;
};
