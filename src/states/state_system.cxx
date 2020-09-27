#include "state_system.hxx"

void j_state_system::push(j_state_id state_id) {
    switch (state_id) {
    case j_state_id::world:
        current_ = states_.emplace_back(new j_world_state()).get();
        break;
    default:
        LOG(ERROR) << "Unknown state id " << static_cast<uint32_t> (state_id);
        return;
    }

    current_->on_enter();
}

void j_state_system::pop() {
    if (!current_) {
        LOG(ERROR) << "Cannot pop state from an empty stack";
        return;
    }
    current_->on_leave();
    states_.pop_back();
}

void j_state_system::on_load() {
    push(j_state_id::world);
}

void j_state_system::update(uint32_t delta_time) {
    if (current_) {
        current_->on_update();
    }
}

void j_state_system::render(j_display& display) {
    if (current_) {
        current_->on_render(display);
    }
}
