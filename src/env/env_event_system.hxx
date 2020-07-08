#ifndef JARALYN_ENV_EVENT_SYSTEM_HXX
#define JARALYN_ENV_EVENT_SYSTEM_HXX

#include "env_interface.hxx"
#include "window.hxx"

/**
 * @brief Manages OS messages
 */
class j_env_event_system {
private:
    std::unique_ptr<j_window> window_ { nullptr };
    std::unique_ptr<entt::dispatcher> dispatcher_ { std::make_unique<entt::dispatcher>() };
public:
    /**
     * @brief Processes all pending SDL2 events and converts them to env events.
     */
    void listen();

    [[nodiscard]] entt::dispatcher& dispatcher() noexcept;
};

#endif
