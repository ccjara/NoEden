#ifndef JARALYN_GAME_EVENT_HXX
#define JARALYN_GAME_EVENT_HXX

#include "gfx/display.hxx"
#include "components/inventory.hxx"
#include "scripts/script.hxx"
#include "scenes/scene_interface.hxx"

enum class j_game_event_type {
    inventory_item_added,
    scene_render,
};

/**
 * @brief Maps strings to j_game_event_type
 *
 * Used when interfacing lua and C++. The string representation should
 * only be used in lua and the enum should be used in C++ at all times.
 */
static const std::unordered_map<std::string_view, j_game_event_type> event_type_by_string = {
    { "INVENTORY_ITEM_ADDED", j_game_event_type::inventory_item_added },
    { "SCENE_RENDER", j_game_event_type::scene_render },
};

class j_inventory_item_added_event {
private:
    j_item* item_ { nullptr };
public:
    constexpr j_inventory_item_added_event(j_item* item) :
        item_ { item } {
        assert(item_);
    }

    [[nodiscard]] constexpr const j_item* item() const noexcept {
        return item_;
    }
};

class j_scene_render_event {
private:
    j_display* display_ { nullptr };
    j_scene_type scene_type_ { j_scene_type::null };
public:
    constexpr j_scene_render_event(j_scene_type scene_type, j_display* display) :
        display_(display),
        scene_type_(scene_type) {
        assert(display);
    }

    [[nodiscard]] constexpr j_display* display() const noexcept {
        return display_;
    }

    [[nodiscard]] constexpr j_scene_type scene_type() const noexcept {
        return scene_type_;
    }
};

#endif