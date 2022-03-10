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
    explicit Input(EventManager& events);

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
    EventManager& events_;
};

#endif
