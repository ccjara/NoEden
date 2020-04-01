#ifndef JARALYN_PLATFORM_MANAGER_HXX
#define JARALYN_PLATFORM_MANAGER_HXX

#include "../manager.hxx"
#include "../env/env_manager.hxx"
#include "platform_events.hxx"
#include "window.hxx"

class platform_manager : public manager<platform_manager> {
private:
    friend class engine;
    std::unique_ptr<window> window_;
    std::unique_ptr<entt::dispatcher> events_ = std::make_unique<entt::dispatcher>();

    void startup(const env_manager& env);
    void shutdown() noexcept;

    uint32_t max_threads_ { 0 };
public:
    void process_events(env_manager& env);
    uint32_t max_threads() const noexcept;

    const window& get_window() const noexcept;
};

#endif
