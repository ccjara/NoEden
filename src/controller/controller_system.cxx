#include "controller_system.hxx"

void j_controller_system::on_load() {
    create<j_unit_controller>();
}

void j_controller_system::update(uint32_t delta_time) {
    for (auto& controller : controllers_) {
        if (!controller->destroyed_) {
            controller->on_update(delta_time);
        }
    }
    // TODO: erase destroyed controllers
}