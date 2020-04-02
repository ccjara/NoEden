#include "../../engine/framework/game_system.hxx"
#include "../../engine/managers/platform/platform_events.hxx"
#include "mouse.hxx"

class test_task : public task {
public:
    void run() override {
    }
};

class input_system : public game_system {
private:
    mouse_t mouse_;

    void on_mouse_move(const mouse_move_event& e);
    void on_mouse_click(const mouse_click_event& e);
public:
    virtual void attach(entt::dispatcher& dispatcher) override;

    const mouse_t& mouse() const noexcept;
};
