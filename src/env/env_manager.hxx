#ifndef JARALYN_ENV_MANAGER_HXX
#define JARALYN_ENV_MANAGER_HXX

#include "../resource/resource_loader.hxx"
#include "clock.hxx"
#include "env_event_dispatcher.hxx"
#include "root_config.hxx"
#include "window.hxx"

class j_env_manager {
private:
    j_clock clock_;
    j_env_event_dispatcher dispatcher_;
    uint32_t max_threads_ { 0 };
    std::unique_ptr<j_window> window_ { nullptr };
    std::unique_ptr<j_root_config> root_config_ { nullptr };

    bool is_running_ { false };

    void on_quit_event(const j_quit_event&);
public:
    const j_root_config& config() const noexcept;
    bool is_running() const noexcept;
    void stop() noexcept;

    void startup(j_resource_loader& res_loader);
    void shutdown() noexcept;

    j_clock& clock() noexcept;
    j_env_event_dispatcher& dispatcher() noexcept;
    j_window& window() noexcept;
};

#endif
