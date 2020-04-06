#include "scene_factory_impl.hxx"

std::unique_ptr<universal_scene> scene_factory_impl::create(scene_id_t id) {
    scene_definition def;

    switch (id) {
    case scene_id::dungeon:
        def.required_systems.push_back(system_id::gfx);
        def.required_systems.push_back(system_id::input);
        def.required_systems.push_back(system_id::grid);
        def.required_systems.push_back(system_id::unit);
        break;
    default:
        LOG(ERROR) << "Unexpected scene id " << id;
        throw;
    }

    return std::make_unique<universal_scene>(std::move(def));
}
