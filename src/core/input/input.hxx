#ifndef JARALYN_INPUT_SYSTEM_HXX
#define JARALYN_INPUT_SYSTEM_HXX

#include "input_state.hxx"
#include "../../event/platform_event.hxx"

/**
 * @brief Input core module, accessed by the engine
 */
class Input {
public:
    explicit Input(entt::dispatcher& dispatcher) : dispatcher_ { dispatcher } {
    }

    /**
     * @brief Processes the platform specific message queue for user input.
     *
     * Buffers the input state and emits platform events based on
     * the messages processed.
     */
    void poll_platform();
private:
    InputState state_;

    entt::dispatcher& dispatcher_;
};

#endif
