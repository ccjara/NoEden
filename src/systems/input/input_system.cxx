#include "input_system.hxx"

void input_system::on_mouse_move(const mouse_move_event& e) {
    mouse_.move(e.pos);
}

void input_system::on_mouse_click(const mouse_click_event& e) {
    LOG(DEBUG) << "Mouse Click: " << static_cast<int> (e.button);

    // TODO: mouse button state
    tasks_.push_back(std::make_unique<test_task>());
}

const mouse_t& input_system::mouse() const noexcept {
    return mouse_;
}

void input_system::attach(entt::dispatcher& dispatcher) {
    dispatcher.sink<mouse_move_event>().connect<&input_system::on_mouse_move>(this);
    dispatcher.sink<mouse_click_event>().connect<&input_system::on_mouse_click>(this);
}
