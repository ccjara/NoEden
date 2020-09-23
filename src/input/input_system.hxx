#ifndef JARALYN_INPUT_SYSTEM_HXX
#define JARALYN_INPUT_SYSTEM_HXX

#include "../event/platform_event.hxx"
#include "../system.hxx"
#include "mouse.hxx"
#include "keyboard.hxx"

/**
 * @brief Facade managing all game input
 */
class j_input_system : public j_system<j_input_system> {
private:
    j_keyboard keyboard_;
    j_mouse mouse_;
public:
    /**
     * @brief Returns the current, readonly mouse state
     */
    const j_mouse_state& mouse() const;

    /**
     * @brief Returns the current, readonly keyboard state
     */
    const j_keyboard_state& keyboard() const;

    void update(uint32_t delta_time) override;
};

#endif
