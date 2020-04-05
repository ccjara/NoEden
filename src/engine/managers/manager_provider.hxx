#ifndef JARA_MANAGER_PROVIDER_HXX
#define JARA_MANAGER_PROVIDER_HXX

#include "resource/resource_manager.hxx"
#include "env/env_manager.hxx"
#include "platform/platform_manager.hxx"
#include "task/task_manager.hxx"

class task_manager;
class platform_manager;
class env_manager;
class resource_manager;

struct manager_provider {
private:
    friend class engine;

    bool is_running = false;
public:
    const std::unique_ptr<resource_manager> resource = std::make_unique<resource_manager>();
    const std::unique_ptr<env_manager> env = std::make_unique<env_manager>();
    const std::unique_ptr<platform_manager> platform = std::make_unique<platform_manager>();
    const std::unique_ptr<task_manager> task = std::make_unique<task_manager>();

    /**
     * @brief Starts all manager subsystems
     *
     * The order is specific and must be kept in sync with the shutdown order
     */
    void startup();

    /**
     * @brief Stops all manager subsystems
     *
     * The order is specific and must be kept in sync with the shutdown order
     */
    void shutdown();

    manager_provider() = default;
    ~manager_provider();

    manager_provider(const manager_provider&) = delete;
    manager_provider(manager_provider&&) = delete;
    manager_provider& operator=(manager_provider&&) = delete;
    const manager_provider& operator=(const manager_provider&) = delete;
};

#endif
