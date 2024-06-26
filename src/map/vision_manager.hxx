#ifndef NOEDEN_VISION_MANAGER_HXX
#define NOEDEN_VISION_MANAGER_HXX

#include "world/world_event.hxx"

class IEntityReader;
class ITileReader;

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
    EventResult on_world_ready(const WorldReadyEvent& e);
    Subscription<WorldReadyEvent> world_ready_sub_;
    EventResult on_world_updated_pre(const WorldUpdatedPreEvent& e);
    Subscription<WorldUpdatedPreEvent> world_updated_pre_sub_;
    EventResult on_world_updated_post(const WorldUpdatedPostEvent& e);
    Subscription<WorldUpdatedPostEvent> world_updated_post_sub_;

    EventManager* events_ = nullptr;
    IEntityReader* entity_reader_ = nullptr;
    ITileReader* tile_reader_ = nullptr;
};

#endif
