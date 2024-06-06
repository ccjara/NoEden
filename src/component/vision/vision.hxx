#ifndef NOEDEN_VISION_HXX
#define NOEDEN_VISION_HXX

#include "component/generic_component.hxx"
#include "component/vision/fov.hxx"

class ITileReader;

/**
 * @brief Represents the vision of an entity. Any entity with this component can see.
 */
class Vision : public GenericComponent<Vision, ComponentType::Vision> {
public:
    explicit Vision(ITileReader* tile_reader);

    /**
     * @brief Provides a const reference to the contained FoV
     */
    const Fov& fov() const;

    /**
     * @brief Returns a mutable reference to the contained FoV
     */
    Fov& fov();

    /**
     * @brief Sets the vision radius of the entity
     */
    void set_vision_radius(i32 radius);

    /**
     * @brief Returns the vision radius of the entity
     */
    i32 vision_radius() const;

    /**
     * @brief Returns whether the field of view should be applied to the map
     */
    bool applied() const;

    /**
     * @brief Sets whether the field of view should be applied to the map
     */
    void set_apply(bool apply_vision);

    void on_player_attached() override;
    void on_player_detached() override;
private:
    Fov fov_;

    /**
     * @brief Whether the FoV should be applied to the map
     */
    bool apply_ = false;

    ITileReader* tile_reader_ = nullptr;
};

#endif