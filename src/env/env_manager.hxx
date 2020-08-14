#ifndef JARALYN_ENV_MANAGER_HXX
#define JARALYN_ENV_MANAGER_HXX

#include "clock.hxx"
#include "env_event_dispatcher.hxx"
#include "root_config.hxx"
#include "window.hxx"
#include "../event/event.hxx"
#include "../event/event_listener.hxx"
#include "../scripts/script_system.hxx"

/**
 * @brief Facade around the game platform (OS) and its specificities
 */
class j_env_manager : public j_event_listener {
private:
    j_clock clock_;
    entt::dispatcher* dispatcher_;

    std::unique_ptr<j_window> window_ { nullptr };
    std::unique_ptr<j_env_event_dispatcher> env_event_dispatcher_ { nullptr };

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
    void update_root_config(j_script& sys_script) noexcept;
public:
    j_env_manager(entt::dispatcher* const dispatcher);

    /**
     * @brief Frees resources in case {@see stop} was not invoked
     */
    ~j_env_manager() noexcept;

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
    bool running() const noexcept;

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
     * This causes the game loop to break after the current cycle.
     *
     * @see running
     */
    void stop() noexcept;

    /**
     * Polls for pending OS messages and dispatches env events
     *
     * @see env_event_dispatcher
     */
    void process_os_messages() const noexcept;

    void attach(entt::dispatcher& dispatcher) noexcept override;

    j_clock& clock() noexcept;
    j_window& window() noexcept;
};

#endif
