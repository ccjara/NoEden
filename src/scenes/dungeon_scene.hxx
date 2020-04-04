#ifndef JARALYN_DUNGEON_SCENE_HXX
#define JARALYN_DUNGEON_SCENE_HXX

#include "../engine/framework/scene.hxx"

class dungeon_scene : public scene {
protected:
    const std::list<system_id_t> list_required_systems() const override {
        return {
            static_cast<system_id_t> (system_id::gfx),
            static_cast<system_id_t> (system_id::input)
        };
    }
};

#endif