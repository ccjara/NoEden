#include "system_scene.hxx"

void system_scene::create_object(object&& obj) {
    objects_[obj.type()].emplace_back(&obj);
}
