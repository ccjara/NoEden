#ifndef NOEDEN_WORLD_HXX
#define NOEDEN_WORLD_HXX

#include "action/action_event.hxx"
#include "world/camera_controller.hxx"
#include "world/tile_accessor.hxx"
#include "realm/realm.hxx"
#include "world_context.hxx"

class ChunkGenerator;
class Renderer;

class WorldRealm : public Realm {
public:
    WorldRealm();
    ~WorldRealm() override;

    bool initialize() override;
    void load() override;
    void update() override;

    const Camera& get_camera() const;

    CameraController& get_camera_controller();

    WorldContext& world_context();
private:
    EventResult on_player_action_committed(const PlayerActionCommitted& e);
    Subscription<PlayerActionCommitted> player_action_committed_sub_;

    Renderer* renderer_ = nullptr;

    std::unique_ptr<WorldContext> world_context_ = nullptr;
    std::unique_ptr<WorldSpec> world_spec_ = nullptr;
    std::unique_ptr<ActionQueue> action_queue_ = nullptr;
    std::unique_ptr<EntityManager> entity_manager_ = nullptr;
    std::unique_ptr<ChunkManager> chunk_manager_ = nullptr;
    std::unique_ptr<ChunkGenerator> chunk_generator_ = nullptr;
    std::unique_ptr<Camera> camera_ = nullptr;
    std::unique_ptr<CameraController> camera_controller_ = nullptr;
    std::unique_ptr<PlayerController> player_controller_ = nullptr;
    std::unique_ptr<TileAccessor> tile_accessor_ = nullptr;
    std::unique_ptr<VisionManager> vision_manager_ = nullptr;
};

#endif
