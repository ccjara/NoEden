#include "game_system.hxx"

std::vector<std::unique_ptr<task>>& game_system::submit_tasks() {
    return tasks_;
};

void game_system::attach(entt::dispatcher& dispatcher) {
}
