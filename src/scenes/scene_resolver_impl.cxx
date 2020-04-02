#include "scene_resolver_impl.hxx"

std::unique_ptr<scene> scene_resolver_impl::resolve_scene_by_id(scene_id_t id) {
    switch (static_cast<scene_id> (id)) {
    case scene_id::dungeon:
        return std::make_unique<dungeon_scene>();
    default:
        LOG(ERROR) << "Unexpected scene id " << id;
        throw;
    }
}
