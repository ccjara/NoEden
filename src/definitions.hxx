#ifndef JARALYN_DEFINITIONS_HXX
#define JARALYN_DEFINITIONS_HXX

enum class j_system_id {
    gfx,
    input,
};

enum class j_mouse_button {
    left = SDL_BUTTON_LEFT,
    middle = SDL_BUTTON_MIDDLE,
    right = SDL_BUTTON_RIGHT,
};

struct j_color
{
    unsigned char r { 255 };
    unsigned char g { 255 };
    unsigned char b { 255 };
    unsigned char a { 255 };

    j_color() = default;

    j_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
     : r(r), g(g), b(b), a(a) {
    }
};

template<typename t>
struct j_vec3 {
    t x { 0 };
    t y { 0 };
    t z { 0 };
};

#endif
