#ifndef JARALYN_SCENE_FACTORY_HXX
#define JARALYN_SCENE_FACTORY_HXX

#include "scene.hxx"

class scene_factory {
public:
    /**
     * @brief Instantiates a scene based on the given scene id
     *
     * The scene id is implementation specific. This method will be called
     * Prior to executing the game loop and may be called by the engine
     * to commence a scene switch, which may occur based on game state changes.
     */
    [[nodiscard]] virtual std::unique_ptr<scene> create(scene_id_t id) = 0;

    /**
     * @brief Returns the initial, game specific scene to load
     *
     * This scene will be loaded prior to entering the game loop.
     */
    virtual scene_id_t get_initial_scene_id() const noexcept = 0;

    /**
     * @brief Creates the initially configured scene.
     *
     * This is a shortcut to pairing get_initial_scene_id() and create().
     */
    std::unique_ptr<scene> create_initial_scene() {
        return create(get_initial_scene_id());
    }

    virtual ~scene_factory() = default;
};

#endif
