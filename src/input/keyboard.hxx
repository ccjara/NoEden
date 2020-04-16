#ifndef JARALYN_KEYBOARD_HXX
#define JARALYN_KEYBOARD_HXX

#include "input_interface.hxx"

class j_keyboard : public j_keyboard_state {
private:
    std::unordered_map<SDL_Keycode, bool> keys_;
public:
    j_keyboard();

    /**
     * @brief Returns whether the given keycode is currently pressed
     */
    bool is_pressed(SDL_Keycode key) const noexcept override;

    /**
     * @brief Sets the given key as pressed
     */
    void key_down(SDL_Keycode key);

    /**
     * @brief Sets the given key as released
     */
    void key_up(SDL_Keycode key);
};

#endif
