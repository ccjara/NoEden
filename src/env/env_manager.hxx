#ifndef JARALYN_ENV_MANAGER_HXX
#define JARALYN_ENV_MANAGER_HXX

#include "clock.hxx"
#include "env_interface.hxx"
#include "env_event_system.hxx"
#include "root_config.hxx"
#include "window.hxx"
#include "../scripts/script_system.hxx"

/**
 * @brief Facade around the game platform (OS) and its specificities
 */
class j_env_manager {
private:
    j_clock clock_;
    j_env_event_system events_;
    uint32_t max_threads_ { 0 };
    std::unique_ptr<j_window> window_ { nullptr };
    std::unique_ptr<j_root_config> root_config_ { nullptr };

    bool is_running_ { false };

    void on_quit_event(const j_quit_event&);
    void on_resize_event(const j_resize_event&);
public:
    /**
     * @brief Returns the root config
     */
    const j_root_config& config() const noexcept;

    /**
     * @brief Returns whether the game is still running
     *
     * When false, the game loop will break after the current cycle,
     * effectively triggering the exit routine.
     */
    bool is_running() const noexcept;

    /**
     * @brief Request a stop of the environment
     *
     * This causes the game loop to break after the current cycle.
     *
     * @see is_running
     */
    void stop() noexcept;

    /**
     * @brief Sets up this service
     *
     * Loads the configuration, reads hardware capabilities and initializes 
     * the platform (using SDL2) thereafter by creating the game window.
     */
    void startup(j_script_system& scripts);

    /**
     * @brief Shuts down the platform (SDL2)
     *
     * Must be called _after_ {@link stop} was called
     */
    void shutdown() noexcept;

    j_clock& clock() noexcept;
    j_env_event_system& events() noexcept;
    j_window& window() noexcept;
};

#endif
