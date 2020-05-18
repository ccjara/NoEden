#include "../env/env_interface.hxx"
#include "input_interface.hxx"
#include "mouse.hxx"
#include "keyboard.hxx"

class j_input_system : public j_input_state, public j_env_event_listener {
protected:
    j_keyboard keyboard_;
    j_mouse mouse_;

    void on_mouse_move(const j_mouse_move_event& e);
    void on_mouse_down(const j_mouse_down_event& e);
    void on_mouse_up(const j_mouse_up_event& e);

    void on_key_down(const j_key_down_event& e);
    void on_key_up(const j_key_up_event& e);
public:
    j_mouse_state& mouse() noexcept override;
    j_keyboard_state& keyboard() noexcept override;

    virtual void attach(entt::dispatcher& dispatcher) override;
};
