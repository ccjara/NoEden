#ifndef JARALYN_MOUSE_BUTTON_HXX
#define JARALYN_MOUSE_BUTTON_HXX

enum class MouseButton {
    Left = SDL_BUTTON_LEFT,
    Middle = SDL_BUTTON_MIDDLE,
    Right = SDL_BUTTON_RIGHT,
};

constexpr uint32_t MOUSE_BUTTON_COUNT { 3U };

#endif
