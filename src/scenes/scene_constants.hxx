#ifndef JARALYN_SCENE_CONSTANTS_HXX
#define JARALYN_SCENE_CONSTANTS_HXX

#include "../systems/system_definition.hxx"

enum class scene_id : scene_id_t {
    dungeon,
};

constexpr auto initial_scene { scene_id::dungeon };

#endif
