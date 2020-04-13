#include "input_system.hxx"

const j_mouse& j_input_system::mouse() const noexcept {
    return mouse_;
}

void j_input_system::on_mouse_move(const j_mouse_move_event& e) {
    mouse_.move(e.position);
}

void j_input_system::on_mouse_down(const j_mouse_down_event& e) {
    mouse_.mouse_down(e.button);
    LOG(DEBUG) << "Mouse Press: " << static_cast<int> (e.button);
}

void j_input_system::on_mouse_up(const j_mouse_up_event& e) {
    mouse_.mouse_up(e.button);
    LOG(DEBUG) << "Mouse Release: " << static_cast<int> (e.button);
}

void j_input_system::attach(entt::dispatcher& dispatcher) {
    dispatcher.sink<j_mouse_move_event>().connect<&j_input_system::on_mouse_move>(this);
    dispatcher.sink<j_mouse_down_event>().connect<&j_input_system::on_mouse_down>(this);
    dispatcher.sink<j_mouse_up_event>().connect<&j_input_system::on_mouse_up>(this);
    LOG(INFO) << "Input system now listening to env events";
}
