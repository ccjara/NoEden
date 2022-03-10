#ifndef JARALYN_GAME_HXX
#define JARALYN_GAME_HXX

#include "config.hxx"
#include "window.hxx"
#include "../input/input.hxx"
#include "../ui/ui.hxx"
#include "../scene/scene.hxx"
#include "../scene/scene_events.hxx"
#include "../scripts/script.hxx"
#include "../scripts/script_event.hxx"
#include "../game/platform_event.hxx"
#include "../gfx/renderer.hxx"
#include "../actor/player_controller.hxx"
#include "../actor/action_queue.hxx"
#include "../scripts/scripting.hxx"
#include "../xray/xray.hxx"

/**
 * @brief Facade around the game platform (OS) and its specificities
 */
class Game {
    friend class GameFactory;
public:
    Game();

    /**
     * @brief Returns whether the game is still running.
     *
     * When false, the game loop will break after the current cycle,
     * effectively triggering the exit routine.
     */
    bool running() const;

    /**
     * @brief Starts the environment.
     *
     * Loads the configuration, reads hardware capabilities and initializes
     * the platform (using SDL2) thereafter by creating the game window.
     */
    void start();

    /**
     * @brief Request a stop of the environment.
     *
     * Frees owned resources and causes the game loop to break after the
     * current cycle.
     *
     * @see running
     */
    void stop();

    /**
     * @brief Runs the game loop until stopped by the player.
     * 
     * @see stop
     */
    void run();

    bool on_mouse_down(MouseDownEvent& e) {
        if (input_.state().is_mouse_pressed(MouseButton::Left)) {
            const auto mp = input_.state().mouse_position();
            const Vec2<u32> tp = { mp.x / 16, mp.y / 28 };

            if (scene_.tiles().at(tp)) {
                auto w = TileBuilder::wall();
                w.revealed = true;
                scene_.tiles().put(w, tp);
                scene_.update_fov(player_controller_.player());
            }
        } else if (input_.state().is_mouse_pressed(MouseButton::Right)) {
            const auto mp = input_.state().mouse_position();
            const Vec2<u32> tp = { mp.x / 16, mp.y / 28 };

            if (scene_.tiles().at(tp)) {
                auto f = TileBuilder::floor();
                f.revealed = true;
                scene_.tiles().put(f, tp);
                scene_.update_fov(player_controller_.player());
            }
        }
        return false;
    }
private:
    u32 last_ticks_;

    EventManager events_;
    PlayerController player_controller_;
    ActionQueue action_queue_;
    Scene scene_;
    Scripting scripting_;

    Input input_;
    Window window_;
    Renderer renderer_;
    Ui ui_;
    Xray xray_;

    /**
     * @brief The source of truth of the system configuration.
     *
     * Some services may claim a copy of this config which gets distributed by
     * the dispatcher.
     *
     * @see ConfigUpdatedEvent
     */
    Config config_;

    /**
     * @brief Terminator for the game loop.
     */
    bool is_running_ { false };


    /**
     * @brief Polls for pending OS messages and dispatches env events.
     *
     * @see env_event_dispatcher
     */
    void process_os_messages();

    bool on_script_loaded(ScriptLoadedEvent&);

    /**
     * @brief Updates the config from inside a lua script.
     */
    void configure_from_lua(luabridge::LuaRef cfg);
};

#endif
