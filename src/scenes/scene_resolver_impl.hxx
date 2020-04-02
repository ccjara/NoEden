#ifndef JARALYN_SCENE_RESOLVER_IMPL_HXX
#define JARALYN_SCENE_RESOLVER_IMPL_HXX

#include "../engine/framework/scene_resolver.hxx"
#include "../engine/framework/scene.hxx"

#include "scene_constants.hxx"
#include "dungeon_scene.hxx"

/**
 * @brief Provides game specific scene resolving
 */
class scene_resolver_impl : public scene_resolver {
public:
    std::unique_ptr<scene> resolve_scene_by_id(scene_id_t id) override;

    scene_id_t get_initial_scene_id() const noexcept override {
        return static_cast<scene_id_t> (initial_scene);
    }
};

#endif
