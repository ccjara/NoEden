#ifndef JARALYN_VISION_MANAGER_HXX
#define JARALYN_VISION_MANAGER_HXX

class EventManager;
class IEntityReader;
class ITileReader;
class WorldReadyEvent;
class WorldUpdatedPreEvent;
class WorldUpdatedPostEvent;

class VisionManager {
public:
    explicit VisionManager(IEntityReader *entity_reader, ITileReader* tile_reader, EventManager* events);

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

    EventManager* events_ = nullptr;
    IEntityReader* entity_reader_ = nullptr;
    ITileReader* tile_reader_ = nullptr;
};

#endif
