#include "world/world_realm.hxx"

#include "action/action.hxx"
#include "action/action_queue.hxx"
#include "catalog/catalog.hxx"
#include "component/render.hxx"
#include "entity/entity.hxx"
#include "entity/entity_manager.hxx"
#include "gfx/renderer.hxx"
#include "player_controller.hxx"
#include "realm/realm_type_def.hxx"
#include "world/camera.hxx"
#include "world/chunk.hxx"
#include "world/chunk_generator.hxx"
#include "world/chunk_manager.hxx"
#include "world/tile.hxx"
#include "world/world_event.hxx"
#include "world/world_spec_creator.hxx"
#include "world/world_spec.hxx"
#include "world/vision_manager.hxx"

WorldRealm::WorldRealm() : Realm(RealmType::World) {}

WorldRealm::~WorldRealm() {}

bool WorldRealm::initialize() {
    renderer_ = services_->get<Renderer>();
    if (!renderer_) {
        return false;
    }
    world_context_ = std::make_unique<WorldContext>();

    action_queue_ = std::make_unique<ActionQueue>();
    chunk_manager_ = std::make_unique<ChunkManager>();
    camera_ = std::make_unique<Camera>();
    camera_controller_ = std::make_unique<CameraController>();
    entity_manager_ = std::make_unique<EntityManager>();
    player_controller_ =std::make_unique<PlayerController>();
    tile_accessor_ = std::make_unique<TileAccessor>();
    vision_manager_ = std::make_unique<VisionManager>();

    world_context_->action_queue = action_queue_.get();
    world_context_->chunk_manager = chunk_manager_.get();
    world_context_->camera = camera_.get();
    world_context_->camera_controller = camera_controller_.get();
    world_context_->entity_manager = entity_manager_.get();
    world_context_->tile_accessor = tile_accessor_.get();
    world_context_->events = events_;
    world_context_->services = services_;
    world_context_->vision_manager = vision_manager_.get();

    action_queue_->initialize(world_context_.get());

    if (!chunk_manager_->initialize(world_context_.get())) {
        LOG_ERROR("Could not initialize ChunkManager");
        return false;
    }

    camera_controller_->initialize(world_context_.get());
    entity_manager_->initialize(world_context_.get());
    player_controller_->initialize(world_context_.get());
    tile_accessor_->initialize(world_context_.get());
    vision_manager_->initialize(world_context_.get());

    player_action_committed_sub_ = events_->on<PlayerActionCommitted>(this, &WorldRealm::on_player_action_committed);

    return true;
}

EventResult WorldRealm::on_player_action_committed(const PlayerActionCommitted& e) {
    if (e.action == nullptr) [[unlikely]] {
        return EventResult::Continue;
    }
    Profiler::timer("Update").start();

    events_->trigger<WorldUpdatedPreEvent>();

    // give all entities in the world energy according to the cost of the action
    const auto cost = e.action->cost();
    for (auto& entity : entity_manager_->entities()) {
        entity->update(cost);
    }

    // process actions in order
    action_queue_->process_actions();

    // inform entities that actions have been processed
    for (auto& entity : entity_manager_->entities()) {
        entity->on_after_actions();
    }

    events_->trigger<WorldUpdatedPostEvent>();
    Profiler::timer("Update").stop();

    return EventResult::Continue;
}

const Camera& WorldRealm::get_camera() const { return *camera_; }

CameraController& WorldRealm::get_camera_controller() { return *camera_controller_; }

void WorldRealm::load() {
    // temp code
    WorldSpecCreator world_spec_creator;

    world_spec_ = world_spec_creator.create_world_spec(CreateWorldSpecOptions {
        .seed = 0,
        .chunks_x = 256,
        .chunks_z = 256,
        .max_vegetation = static_cast<i32>(0.8f * Chunk::CHUNK_DEPTH),
        .max_shoreline = static_cast<i32>(0.45f * Chunk::CHUNK_DEPTH),
        .max_water = static_cast<i32>(0.4f * Chunk::CHUNK_DEPTH)
    });

    events_->trigger<WorldReadyEvent>(world_spec_.get());

    camera_controller_->control(camera_.get());

    auto* catalog = services_->get<ResourceManager>()->catalog();
    auto arch_troll = catalog->archetype("TROLL");
    auto arch_human = catalog->archetype("HUMAN");
    if (arch_troll) {
        entity_manager_->create_entity(*arch_troll, WorldPos(3, 0, 3));
    }
    else {
        LOG_WARN("TROLL archetype not yet present");
    }
    if (arch_human) {
        auto& human = entity_manager_->create_entity(*arch_human, WorldPos(1, 0, 1));
        entity_manager_->set_controlled_entity(&human);
        camera_controller_->set_target(&human);
    }
    else {
        LOG_WARN("HUMAN archetype not yet present");
    }
}

void WorldRealm::update() {
    // TODO: temporary code
    Profiler::timer("Draw").start();
    auto& world_layer = renderer_->display();

    world_layer.reset();

    const auto cam_pos = camera_->position;
    i32 display_half_width = world_layer.dimensions().x / 2;
    i32 display_half_height = world_layer.dimensions().y / 2;

    const i32 left_bound = cam_pos.x - display_half_width;
    const i32 right_bound = cam_pos.x + display_half_width;
    const i32 top_bound = cam_pos.z - display_half_height;
    const i32 bottom_bound = cam_pos.z + display_half_height;

    for (i32 z = top_bound; z <= bottom_bound; ++z) {
        for (i32 x = left_bound; x <= right_bound; ++x) {
            Tile* tile = tile_accessor_->get_tile(WorldPos(x, cam_pos.y, z));
            if (tile && tile->flags.test(Revealed)) {
                auto tile_pos = glm::ivec2(x - left_bound, z - top_bound);

                world_layer.put(tile->display_info, tile_pos);
            }
        }
    }

    /*
    Grid<Tile>& tiles { tile_manager_->tiles() };
    const auto scene_dim { tiles.dimensions() };
    const auto display_dim { world_layer.dimensions() };
    const Vec2<u32> tile_render_dim (
        std::min<u32>(scene_dim.x, display_dim.x),
        std::min<u32>(scene_dim.y, display_dim.y)
    );
    for (u32 y = 0; y < tile_render_dim.y; ++y) {
        for (u32 x = 0; x < tile_render_dim.x; ++x) {
            const Vec2<u32> tile_pos { x, y };
            const Tile* tile { tiles.at(tile_pos) };

            auto cell = tile->display_info;

            // logic does not work with mutliple layers (game + ui)
            // the ui will be "burned in"
            // TODO: draw world and ui into separate buffers and then merge them

            if (tile->flags.test(TileFlags::FoV)) {
                world_layer.put(std::move(cell), tile_pos);
            } else {
                world_layer.at(tile_pos)->color = Color::mono(128);
            }
        }
    }
    */
    for (const auto& entity : entity_manager_->entities()) {
        const auto pos = entity->position - WorldPos(left_bound, 0, top_bound);

        if (!world_layer.in_bounds(glm::ivec2(pos.x, pos.z))) {
            continue;
        }

        auto render_component = entity->component<Render>();
        if (!render_component) {
            continue;
        }
        const auto& info = render_component->display_info();
        if (!info.visible) {
            continue;
        }

        // const auto tile { tiles.at(entity->position) };
        // if (!tile || !tile->flags.test(TileFlags::FoV)) {
        //     continue;
        // }

        if (entity->position.y != cam_pos.y) {
            continue;
        }

        world_layer.put(DisplayCell(info.glyph, info.color), glm::ivec2(pos.x, pos.z));
    }
}

WorldContext& WorldRealm::world_context() { return *world_context_; }
