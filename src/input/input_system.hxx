#include "../env/env_event.hxx"
#include "../env/env_event_listener.hxx"
#include "../system.hxx"
#include "mouse.hxx"

class j_input_system : public j_system, public j_env_event_listener {
protected:
    j_mouse mouse_;

    void on_mouse_move(const j_mouse_move_event& e);
    void on_mouse_down(const j_mouse_down_event& e);
    void on_mouse_up(const j_mouse_up_event& e);
public:
    const j_mouse& mouse() const noexcept;

    virtual void attach(entt::dispatcher& dispatcher) override;
};
