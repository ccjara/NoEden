#ifndef JARALYN_STATE_MANAGER_HXX
#define JARALYN_STATE_MANAGER_HXX

#include "change_controller.hxx"
#include "change_notification.hxx"

class state_manager : public change_controller {
    friend class manager_provider;
private:
    entt::dispatcher dispatcher_;

    void startup();
    void shutdown();
public:
    void notify(change_notification&& n) override;
    void distribute_changes();
};

#endif
