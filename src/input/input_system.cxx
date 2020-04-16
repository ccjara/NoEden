#include "input_system.hxx"

const j_mouse_state& j_input_system::mouse() const noexcept {
    return mouse_;
}

const j_keyboard_state& j_input_system::keyboard() const noexcept {
    return keyboard_;
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

void j_input_system::on_key_down(const j_key_down_event& e) {
    LOG(DEBUG) << "Key Pressed: " << static_cast<int> (e.key);

    keyboard_.key_down(e.key);
}

void j_input_system::on_key_up(const j_key_up_event& e) {
    LOG(DEBUG) << "Key Released: " << static_cast<int> (e.key);

    keyboard_.key_up(e.key);
}

void j_input_system::attach(entt::dispatcher& dispatcher) {
    dispatcher.sink<j_mouse_move_event>().connect<&j_input_system::on_mouse_move>(this);
    dispatcher.sink<j_mouse_down_event>().connect<&j_input_system::on_mouse_down>(this);
    dispatcher.sink<j_mouse_up_event>().connect<&j_input_system::on_mouse_up>(this);
    dispatcher.sink<j_key_down_event>().connect<&j_input_system::on_key_down>(this);
    dispatcher.sink<j_key_up_event>().connect<&j_input_system::on_key_up>(this);
    LOG(INFO) << "Input system now listening to env events";
}
