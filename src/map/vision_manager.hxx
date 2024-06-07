#ifndef NOEDEN_VISION_MANAGER_HXX
#define NOEDEN_VISION_MANAGER_HXX

struct Events;
class IEntityReader;
class ITileReader;
class WorldReadyEvent;
class WorldUpdatedPreEvent;
class WorldUpdatedPostEvent;

class VisionManager {
public:
    explicit VisionManager(IEntityReader *entity_reader, ITileReader* tile_reader, Events* events);

    /**
     * @brief Applies the field of view of all relevant entities to the map
     */
    void apply_fov();

    /**
     * @brief Updates the field of view of all relevant entities
     */
    void update_fov();
private:
    bool on_world_ready(const WorldReadyEvent& e);
    bool on_world_updated_pre(const WorldUpdatedPreEvent& e);
    bool on_world_updated_post(const WorldUpdatedPostEvent& e);

    Events* events_ = nullptr;
    IEntityReader* entity_reader_ = nullptr;
    ITileReader* tile_reader_ = nullptr;
};

#endif
