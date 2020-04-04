#ifndef JARALYN_SCENE_FACTORY_IMPL_HXX
#define JARALYN_SCENE_FACTORY_IMPL_HXX

#include "../engine/framework/scene_factory.hxx"
#include "../engine/framework/scene.hxx"

#include "scene_constants.hxx"
#include "dungeon_scene.hxx"

/**
 * @brief Instantiates a game specific scene
 */
class scene_factory_impl : public scene_factory {
public:
    [[nodiscard]] std::unique_ptr<scene> create(scene_id_t id) override;

    scene_id_t get_initial_scene_id() const noexcept override {
        return static_cast<scene_id_t> (initial_scene);
    }
};

#endif
