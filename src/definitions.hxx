#ifndef JARALYN_DEFINITIONS_HXX
#define JARALYN_DEFINITIONS_HXX

// TODO: move to submodule definitions, include in pch, then remove this file

/**
 * @see scene_type_by_string
 */
enum class j_scene_type {
	null,
	world,
	status,
	inventory
};

/**
 * @brief Maps strings to j_scene_type
 *
 * Used when interfacing lua and C++. The string representation should
 * only be used in lua and the enum should be used in C++ at all times.
 */
static const std::unordered_map<std::string_view, j_scene_type> scene_type_by_string {
    { "inventory", j_scene_type::inventory },
};

/**
 * @see event_type_by_string
 */
enum class j_public_event_type {
    inventory_item_added,
};

/**
 * @brief Maps strings to j_public_event_type
 *
 * Used when interfacing lua and C++. The string representation should
 * only be used in lua and the enum should be used in C++ at all times.
 *
 * Not every event is necessarily exposed to lua.
 */
static const std::unordered_map<std::string_view, j_public_event_type> event_type_by_string {
    { "INVENTORY_ITEM_ADDED", j_public_event_type::inventory_item_added },
};

enum class j_mouse_button {
    left = SDL_BUTTON_LEFT,
    middle = SDL_BUTTON_MIDDLE,
    right = SDL_BUTTON_RIGHT,
};

struct j_color {
    unsigned char r { 255 };
    unsigned char g { 255 };
    unsigned char b { 255 };
    unsigned char a { 255 };

    j_color() = default;

    explicit j_color(int32_t value) :
        r { static_cast<unsigned char> ((value >> 24) & 0xFF) },
        g { static_cast<unsigned char> ((value >> 16) & 0xFF) },
        b { static_cast<unsigned char> ((value >> 8) & 0xFF) },
        a { static_cast<unsigned char> (value & 0xFF) } {
    }

    j_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
     : r(r), g(g), b(b), a(a) {
    }
};

#endif
