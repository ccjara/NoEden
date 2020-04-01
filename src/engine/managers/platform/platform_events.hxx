#ifndef JARALYN_PLATFORM_EVENTS_HXX
#define JARALYN_PLATFORM_EVENTS_HXX

enum class mouse_button {
    left = SDL_BUTTON_LEFT,
    middle = SDL_BUTTON_MIDDLE,
    right = SDL_BUTTON_RIGHT,
};

struct platform_event {

};

struct mouse_move_event : public platform_event {
    position<int32_t> pos { 0, 0 };
};

struct mouse_click_event : public platform_event {
    mouse_button button;
};

#endif
