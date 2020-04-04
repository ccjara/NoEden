#ifndef JARALYN_SCENE_LOADER_HXX
#define JARALYN_SCENE_LOADER_HXX

#include "../managers/platform/platform_manager.hxx"
#include "scene.hxx"
#include "scene_factory.hxx"

class scene_loader {
protected:
    scene_factory* const factory_;
    platform_manager* const platform_;

    std::unique_ptr<scene> current_scene_;
public:
    /**
     * @brief Configures the scene loader
     *
     * Accepts a game specific scene_factory which instantiates scenes from ids.
     */
    scene_loader(scene_factory* const factory, platform_manager* const platform);

    /**
     * @brief Loads a scene by id. The scene id is game specific
     */
    void load_scene(scene_id_t id);

    /**
     * @brief Loads the scene which was configured to be loaded initially
     */
    void load_initial_scene();

    /**
     * @brief Returns the currently active scene
     */
    const scene& current_scene() const noexcept;
};

#endif
