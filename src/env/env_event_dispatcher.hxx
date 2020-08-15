#ifndef JARALYN_ENV_EVENT_DISPATCHER_HXX
#define JARALYN_ENV_EVENT_DISPATCHER_HXX

#include "../event/event.hxx"

/**
 * @brief Dispatches env events based on polled OS messages
 */
class j_env_event_dispatcher {
private:
    entt::dispatcher* const dispatcher_ { nullptr };
public:
    /**
     * The dispatcher pointer must outlive the scope of this class
     */
    explicit j_env_event_dispatcher(entt::dispatcher* const dispatcher);

    /**
     * @brief Processes all pending SDL2 events and converts them to env events.
     */
    void listen() const noexcept;
};

#endif
