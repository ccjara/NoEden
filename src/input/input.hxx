#ifndef JARALYN_INPUT_HXX
#define JARALYN_INPUT_HXX

#include "input_state.hxx"
#include "input_event.hxx"
#include "../xray/xray_event.hxx"

/**
 * @brief Input core module, accessed by the engine
 */
class Input {
public:
    explicit Input(entt::dispatcher& dispatcher);

    /**
     * @brief Processes the platform specific message queue for user input.
     *
     * Buffers the input state and emits platform events based on
     * the messages processed.
     */
    void poll_platform();

    /**
     * @brief Provides readonly access to the input state.
     */
    [[nodiscard]] const InputState& state() const;
private:
    InputState state_;
    entt::dispatcher& dispatcher_;

    /**
     * @brief If true, no input will be forwarded the game.
     *
     * This is required by the xray (engine ui) otherwise typing in inputs
     * will also cause the player to move for instance.
     */
    bool input_blocked_ { false };

    /**
     * @brief Updates the input_blocked_ property based on engine ui focus.
     */
    void on_xray_focus(const XrayFocusEvent& e);
};

#endif
