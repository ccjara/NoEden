#ifndef JARALYN_ENV_EVENT_DISPATCHER_HXX
#define JARALYN_ENV_EVENT_DISPATCHER_HXX

#include "window.hxx"
#include "../event/event.hxx"

/**
 * @brief Dispatches env events based on polled OS messages
 */
class j_env_event_dispatcher {
private:
    j_window* const window_ { nullptr };
    entt::dispatcher* const dispatcher_ { nullptr };
public:
    /**
     * The given pointers must outlive the scope of this class
     */
    j_env_event_dispatcher(j_window* const window, entt::dispatcher* const dispatcher);

    /**
     * @brief Processes all pending SDL2 events and converts them to env events.
     */
    void listen() const noexcept;
};

#endif
