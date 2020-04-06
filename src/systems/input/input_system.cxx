#include "input_system.hxx"

const mouse_t& input_system::mouse() const noexcept {
    return mouse_;
}

namespace detail {
    input_system_impl::input_system_impl() {
        scene_ = std::make_unique<input_scene>();
    }

    void input_system_impl::on_mouse_move(const mouse_move_event& e) {
        mouse_.move(e.pos);
    }

    void input_system_impl::on_mouse_click(const mouse_click_event& e) {
        LOG(DEBUG) << "Mouse Click: " << static_cast<int> (e.button);
    }

    void input_system_impl::attach_platform(entt::dispatcher& dispatcher) {
        LOG(INFO) << "Input system now listening to platform events";

        dispatcher.sink<mouse_move_event>().connect<&input_system_impl::on_mouse_move>(this);
        dispatcher.sink<mouse_click_event>().connect<&input_system_impl::on_mouse_click>(this);
    }
}