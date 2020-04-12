#ifndef JARALYN_ENV_MANAGER_HXX
#define JARALYN_ENV_MANAGER_HXX

#include "../resource/resource_manager.hxx"
#include "clock.hxx"
#include "root_config.hxx"

class resource_manager;

class env_manager {
private:
    friend class manager_provider;

    clock_c clock_;

    bool is_running_ { false };
    std::unique_ptr<root_config> root_config_ { nullptr };

    void startup(resource_manager& resource);
    void shutdown() noexcept;
public:
    const root_config& config() const noexcept;
    bool is_running() const noexcept;
    void stop() noexcept;

    clock_c& clock() noexcept;
};

#endif
