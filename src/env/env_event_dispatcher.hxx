#ifndef JARALYN_ENV_EVENT_HANDLER_HXX
#define JARALYN_ENV_EVENT_HANDLER_HXX

#include "env_event.hxx"
#include "window.hxx"

class j_env_event_dispatcher {
private:
    std::unique_ptr<j_window> window_;
    std::unique_ptr<entt::dispatcher> dispatcher_ = std::make_unique<entt::dispatcher>();
public:
    void listen();

    template<typename env_event_t, auto method, typename instance_t>
    void on(instance_t* listener_instance) {
        dispatcher_->sink<env_event_t>().connect<method>(listener_instance);
    }
};

#endif
