#ifndef JARALYN_ENGINE_HXX
#define JARALYN_ENGINE_HXX

#include "managers/env/env_manager.hxx"
#include "managers/platform/platform_manager.hxx"
#include "managers/resource/resource_manager.hxx"
#include "managers/task/task_manager.hxx"
#include "managers/manager_provider.hxx"

#include "framework/game_system.hxx"
#include "framework/scene_loader.hxx"
#include "framework/scene_resolver.hxx"

class engine {
private:
    std::unique_ptr<resource_manager> resource_ = std::make_unique<resource_manager>();
    std::unique_ptr<env_manager> env_ = std::make_unique<env_manager>();
    std::unique_ptr<platform_manager> platform_ = std::make_unique<platform_manager>();
    std::unique_ptr<task_manager> task_ = std::make_unique<task_manager>();
    std::unique_ptr<scene_loader> scene_loader_;

    manager_provider managers_ = {
        resource_.get(),
        env_.get(),
        platform_.get(),
        task_.get(),
    };

    std::vector<game_system*> systems_;
public:
    engine(scene_resolver* const resolver);

    /**
     * @brief Registers a game system to be used during runtime.
     *
     * The given pointer must outlive the engine as it does not take ownership.
     *
     * You may override the attach method of the system to attach
     * to platform specific events (for example to react to raw
     * mouse motion for your input system)
     *
     * @see game_system::attach()
     */
    void register_system(game_system* sys);

    /**
     * @brief Starts all engine subsystems
     *
     * The order is specific and must be kept in sync with the shutdown order
     */
    void startup() noexcept;

    /**
     * @brief Stops all engine subsystems
     *
     * The order is specific and must be kept in sync with the startup order.
     * Note that the order must be inverse to the startup order (LIFO).
     */
    void shutdown() noexcept;

    /**
     * @brief Runs the engine together will all game systems.
     *
     * Game systems must have been registered before calling this method.
     *
     * @see register_system()
     */
    void run();
};

#endif 