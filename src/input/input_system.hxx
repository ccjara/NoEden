#ifndef JARALYN_INPUT_SYSTEM_HXX
#define JARALYN_INPUT_SYSTEM_HXX

#include "../event/event.hxx"
#include "../system.hxx"
#include "mouse.hxx"
#include "keyboard.hxx"

/**
 * @brief Facade managing all game input
 */
class j_input_system : public j_system<j_input_system>, public j_input_state {
protected:
    j_keyboard keyboard_;
    j_mouse mouse_;
public:
    j_mouse_state& mouse() override;
    j_keyboard_state& keyboard() override;

    void update(uint32_t delta_time) override;
};

#endif
