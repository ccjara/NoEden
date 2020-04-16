#ifndef JARALYN_ENV_EVENT_SYSTEM_HXX
#define JARALYN_ENV_EVENT_SYSTEM_HXX

#include "env_interface.hxx"
#include "window.hxx"

class j_env_event_system {
private:
    std::unique_ptr<j_window> window_;
    std::unique_ptr<entt::dispatcher> dispatcher_ = std::make_unique<entt::dispatcher>();
public:
    void listen();

    entt::dispatcher& dispatcher() noexcept;
};

#endif
