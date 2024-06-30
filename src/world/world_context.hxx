#ifndef NOEDEN_WORLD_CONTEXT_HXX
#define NOEDEN_WORLD_CONTEXT_HXX

class WorldSpec;
class ActionQueue;
class EntityManager;
class ChunkManager;
struct Camera;
class CameraController;
class PlayerController;
class TileAccessor;
class VisionManager;

struct WorldContext {
    WorldSpec* world_spec = nullptr;
    ActionQueue* action_queue = nullptr;
    EntityManager* entity_manager = nullptr;
    ChunkManager* chunk_manager = nullptr;
    Camera* camera = nullptr;
    CameraController* camera_controller = nullptr;
    PlayerController* player_controller = nullptr;
    TileAccessor* tile_accessor = nullptr;
    VisionManager* vision_manager = nullptr;

    EventManager* events = nullptr;
    ServiceLocator* services = nullptr;
};

#endif
