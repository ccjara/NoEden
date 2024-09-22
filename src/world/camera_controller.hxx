#ifndef NOEDEN_CAMERA_CONTROLLER_HXX
#define NOEDEN_CAMERA_CONTROLLER_HXX

#include "world/world_event.hxx"
#include "gfx/gfx_event.hxx"

struct Camera;
class Entity;
struct WorldContext;

class CameraController {
public:
    void initialize(WorldContext* world_context);

    /**
     * @brief Assigns camera to be controlled
     */
    void control(Camera* camera);

    /**
     * @brief Assigns entity to follow
     */
    void set_target(Entity* entity);

    Entity* target() const;
private:
    void adjust(const WorldPos& position);

    EventResult on_key_down(const KeyDownEvent& e);
    Subscription<KeyDownEvent> key_down_sub_ = {};
    EventResult on_world_updated_post(const WorldUpdatedPostEvent& e);
    Subscription<WorldUpdatedPostEvent> world_updated_post_sub_ = {};
    EventResult on_world_ready(const WorldReadyEvent& e);
    Subscription<WorldReadyEvent> world_ready_sub_ = {};
    EventResult on_display_resized(const DisplayResizedEvent& e);
    Subscription<DisplayResizedEvent> display_resized_sub_ = {};

    /**
     * @brief Entity that the camera is following
     */
    Entity* target_ = nullptr;

    Camera* camera_ = nullptr;

    WorldContext* world_context_ = nullptr;

    i32 world_width_ = 0;
    i32 world_height_ = 0;

    glm::ivec2 dispay_size_ = { 0, 0 };
};

#endif
