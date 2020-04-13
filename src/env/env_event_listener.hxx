#ifndef JARALYN_ENV_EVENT_LISTENER_HXX
#define JARALYN_ENV_EVENT_LISTENER_HXX

class j_env_event_listener {
public:
    virtual void attach(entt::dispatcher& dispatcher) = 0;
};

#endif
