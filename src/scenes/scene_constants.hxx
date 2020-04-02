#ifndef JARALYN_SCENE_CONSTANTS_HXX
#define JARALYN_SCENE_CONSTANTS_HXX

#include "../engine/framework/scene.hxx"
#include "../systems/system_definition.hxx"

enum class scene_id : scene_id_t {
    dungeon,
};

constexpr auto initial_scene { scene_id::dungeon };

/**
 * @brief Defines the systems which must be loaded in every scene
 * 
 * These systems do not have to be defined as required.
 *
 * @see required_systems
 */
constexpr auto global_systems {
    std::make_tuple<system_id>(
        system_id::gfx,
        system_id::input
    )
};

const std::map<scene_id, std::tuple<system_id>> required_systems {
};

#endif
