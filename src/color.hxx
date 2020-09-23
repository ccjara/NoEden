#ifndef JARALYN_COLOR_HXX
#define JARALYN_COLOR_HXX

struct j_color {
    unsigned char r { 255 };
    unsigned char g { 255 };
    unsigned char b { 255 };

    constexpr j_color() = default;

    explicit constexpr j_color(int32_t value) :
        r { static_cast<unsigned char> ((value >> 16) & 0xFF) },
        g { static_cast<unsigned char> ((value >> 8) & 0xFF) },
        b { static_cast<unsigned char> (value & 0xFF) } {
    }

    constexpr j_color(unsigned char r, unsigned char g, unsigned char b) :
        r(r), g(g), b(b) {
    }

    constexpr static j_color mono(unsigned char value) {
        return j_color(value, value, value);
    }

    constexpr static j_color white() {
        return j_color::mono(255);
    }

    constexpr static j_color green() {
        return j_color(0, 255, 0);
    }

    constexpr static j_color red() {
        return j_color(255, 0, 0);
    }

    constexpr static j_color yellow() {
        return j_color(255, 255, 0);
    }
};

#endif
