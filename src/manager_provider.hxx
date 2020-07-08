#ifndef JARALYN_MANAGER_PROVIDER_HXX
#define JARALYN_MANAGER_PROVIDER_HXX

#include "resource/resource_manager.hxx"
#include "env/env_manager.hxx"

/**
 * @brief Facade around all game subsystems
 *
 * Todo: probably adds too much complexity
 */
struct j_manager_provider {
private:
    bool is_running = false;
public:
    const std::unique_ptr<j_resource_manager> resource = std::make_unique<j_resource_manager>();
    const std::unique_ptr<j_env_manager> env = std::make_unique<j_env_manager>();

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

    j_manager_provider() = default;
    ~j_manager_provider();

    j_manager_provider(const j_manager_provider&) = delete;
    j_manager_provider(j_manager_provider&&) = delete;
    j_manager_provider& operator=(j_manager_provider&&) = delete;
    const j_manager_provider& operator=(const j_manager_provider&) = delete;
};

#endif
