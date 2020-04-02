#ifndef JARALYN_SCENE_LOADER_HXX
#define JARALYN_SCENE_LOADER_HXX

#include "scene.hxx"
#include "scene_resolver.hxx"

class scene_loader {
protected:
    scene_resolver* const resolver_;

    std::unique_ptr<scene> current_scene_;
public:
    /**
     * @brief Configures the scene loader
     *
     * Accepts a game specific scene_resolver which maps scene ids to scenes.
     */
    explicit scene_loader(scene_resolver* const resolver);

    /**
     * @brief Loads a scene by id. The scene id is game specific
     */
    void load_scene(scene_id_t id);

    /**
     * @brief Loads the scene which was configured to be loaded initially
     *
     * This is a shortcut to calling initial_scene_id() and load_scene()
     * consecutively.
     * 
     * @see initial_scene_id()
     * @see load_scene()
     */
    void load_initial_scene();

    /**
     * @brief Returns the currently active scene
     */
    const scene& current_scene() const noexcept;

    /**
     * @brief Returns the initially configured scene id
     */
    scene_id_t initial_scene_id() const noexcept;
};

#endif
