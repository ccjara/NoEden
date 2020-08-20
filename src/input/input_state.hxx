#ifndef JARALYN_INPUT_INTERFACE_HXX
#define JARALYN_INPUT_INTERFACE_HXX

#include "keyboard_state.hxx"
#include "mouse_state.hxx"

/**
 * @brief Encapsulates all input states inside a single class.
 */
class j_input_state {
public:
    /**
     * @brief Returns the current keyboard state
     */
    virtual j_keyboard_state& keyboard() noexcept = 0;

    /**
     * @brief Returns the current, readonly mouse state
     */
    virtual j_mouse_state& mouse() noexcept = 0;
};

#endif
