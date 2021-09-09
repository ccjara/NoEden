#ifndef JARALYN_COLOR_HXX
#define JARALYN_COLOR_HXX

/**
 * @brief Represents a color which can be displayed by the gfx display
 *
 * This class is globally available as part of the pch.
 */
struct Color {
    unsigned char r { 255 };
    unsigned char g { 255 };
    unsigned char b { 255 };

    constexpr Color() = default;

    explicit constexpr Color(i32 value) {
        assign_from_int(value);
    }

    constexpr Color(unsigned char r, unsigned char g, unsigned char b) :
        r(r), g(g), b(b) {
    }

    constexpr void operator=(i32 value) {
        assign_from_int(value);
    }

    constexpr static Color mono(unsigned char value) {
        return Color(value, value, value);
    }

    constexpr static Color white() {
        return Color::mono(255);
    }

    constexpr static Color green() {
        return Color(0, 255, 0);
    }

    constexpr static Color red() {
        return Color(255, 0, 0);
    }

    constexpr static Color yellow() {
        return Color(255, 255, 0);
    }

    constexpr static Color black() {
        return Color(0, 0, 0);
    }
private:
    constexpr void assign_from_int(i32 value) {
        r = static_cast<unsigned char> ((value >> 16) & 0xFF);
        g = static_cast<unsigned char> ((value >> 8) & 0xFF);
        b = static_cast<unsigned char> (value & 0xFF);
    }
};

#endif
