#ifndef JARALYN_PLATFORM_MANAGER_HXX
#define JARALYN_PLATFORM_MANAGER_HXX

#include "../manager_provider.hxx"
#include "../../framework/system_factory.hxx"
#include "platform_events.hxx"
#include "window.hxx"

class manager_provider;
class system_factory;

class platform_manager {
private:
    friend class manager_provider;
    std::unique_ptr<window> window_;
    std::unique_ptr<entt::dispatcher> events_ = std::make_unique<entt::dispatcher>();

    manager_provider* managers_;

    uint32_t max_threads_ { 0 };

    system_map_t systems_;
    system_factory* sys_factory_ { nullptr };

    void startup(manager_provider* const managers);
    void shutdown() noexcept;
public:
    void process_events();
    uint32_t max_threads() const noexcept;

    void assign_system_factory(system_factory* sys_factory);

    const game_system& load_system(system_id_t id);
    
    const system_map_t& systems() const noexcept;

    const window& get_window() const noexcept;
};

#endif
