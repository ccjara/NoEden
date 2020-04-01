#ifndef JARALYN_GAME_SYSTEM_HXX
#define JARALYN_GAME_SYSTEM_HXX

#include "../engine/framework/task.hxx"
#include "../engine/managers/manager_provider.hxx"

class game_system {
protected:
    std::vector<std::unique_ptr<task>> tasks_;
public:
    std::vector<std::unique_ptr<task>>& submit_tasks() {
        return tasks_;
    };

    virtual void attach(entt::dispatcher& dispatcher) {
    }

    virtual void on_load(const manager_provider& managers) {
    }

    virtual void on_unload() {
    }

    virtual ~game_system() = default;
};

#endif
