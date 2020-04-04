#include "../../engine/framework/game_system.hxx"
#include "../../engine/managers/platform/platform_events.hxx"
#include "../system_definition.hxx"
#include "mouse.hxx"

class test_task : public task {
public:
    void run() override {
    }
};

class input_system : public game_system {
protected:
    mouse_t mouse_;
public:
    const mouse_t& mouse() const noexcept;

    system_id_t id() const noexcept override {
        return static_cast<system_id_t> (system_id::input);
    };
};

namespace detail {
    class input_system_impl : public input_system {
        void on_mouse_move(const mouse_move_event& e);
        void on_mouse_click(const mouse_click_event& e);
    public:
        virtual void attach(entt::dispatcher& dispatcher) override;
    };
}