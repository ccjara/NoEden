#ifndef JARALYN_ENV_MANAGER_HXX
#define JARALYN_ENV_MANAGER_HXX

#include "clock.hxx"
#include "root_config.hxx"
#include "window.hxx"
#include "../event/event.hxx"
#include "../scripts/script_system.hxx"

/**
 * @brief Facade around the game platform (OS) and its specificities
 */
class j_env_manager {
private:
    j_clock clock_;
    entt::dispatcher* const dispatcher_ { nullptr };

    std::unique_ptr<j_window> window_ { nullptr };

    /**
     * @brief The source of truth of the system configuration
     *
     * Some services may claim a copy of this config which gets distributed by
     * the dispatcher.
     *
     * @see j_root_config_updated_event
     */
    j_root_config root_config_;

    bool is_running_ { false };

    void on_quit(const j_quit_event&);
    void on_resize(const j_resize_event&);
    void on_script_loaded(const j_script_loaded_event&);

    /**
     * @brief Updates the root config based on the given script
     *
     * Corrupted config entries will be ignored
     */
    void update_root_config(j_script& sys_script);
public:
    j_env_manager(entt::dispatcher* const dispatcher);

    /**
     * @brief Frees resources in case {@see stop} was not invoked
     */
    ~j_env_manager();

    /**
     * @brief Returns the root config
     */
    const j_root_config& config() const;

    /**
     * @brief Returns whether the game is still running
     *
     * When false, the game loop will break after the current cycle,
     * effectively triggering the exit routine.
     */
    bool running() const;

    /**
     * @brief Starts the environment
     *
     * Loads the configuration, reads hardware capabilities and initializes
     * the platform (using SDL2) thereafter by creating the game window.
     */
    void start();

    /**
     * @brief Request a stop of the environment
     *
     * Frees owned resources and causes the game loop to break after the
     * current cycle.
     *
     * @see running
     */
    void stop();

    /**
     * Polls for pending OS messages and dispatches env events
     *
     * @see env_event_dispatcher
     */
    void process_os_messages() const;

    j_clock& clock();
    j_window& window();
};

#endif
