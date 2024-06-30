#ifndef NOEDEN_VISION_MANAGER_HXX
#define NOEDEN_VISION_MANAGER_HXX

#include "world/world_event.hxx"

struct WorldContext;

class VisionManager {
public:
    void initialize(WorldContext* world_context);

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

    WorldContext* world_context_ = nullptr;
};

#endif
