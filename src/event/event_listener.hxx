#ifndef JARALYN_EVENT_LISTENER_HXX
#define JARALYN_EVENT_LISTENER_HXX

class j_event_listener {
public:
    /**
     * @brief Register an instance to the event dispatcher
     *
     * You can subscribe to multiple events.
     */
    virtual void attach(entt::dispatcher& dispatcher) noexcept = 0;

    virtual ~j_event_listener() = default;
};

#endif
