#include "scene.hxx"

void Scene::init() {
    tiles_.resize({ 120, 40 });
    for (auto& t : tiles_.cells()) {
        t = TileBuilder::floor();
    }

    Events::on<KeyDownEvent>(&Scene::on_key_down);
}

void Scene::shutdown() {
}

Actor* Scene::get_actor_by_id(u64 id) {
    auto it = actor_id_to_index_.find(id);
    if (it == actor_id_to_index_.end()) {
        return nullptr;
    }
    if (it->second >= actors_.size()) {
        return nullptr;
    }
    return actors_[it->second].get();
}

Actor& Scene::create_actor(const Archetype& archetype) {
    actors_.push_back(EntityFactory::create(archetype));
    auto& actor = actors_.back();
    actor_id_to_index_[actor->id] = actors_.size() - 1;
    return *actor.get();
}

Scene::ActorContainer& Scene::actors() {
    return actors_;
}

const Scene::ActorContainer& Scene::read_actors() {
    return actors_;
}

Grid<Tile>& Scene::tiles() {
    return tiles_;
}

const Grid<Tile>& Scene::read_tiles() {
    return tiles_;
}

void Scene::update_fov(u64 id) {
    Actor* actor = get_actor_by_id(id);
    if (!actor) {
        return;
    }
    Fov::update(*actor, tiles_);
}

void Scene::perform_actions() {
    if (!player_id_ || !player_action_) {
        return;
    }
    for (auto& actor : actors_) {
        actor->energy += player_action_->cost;
        // TODO: ai must calculate number of possible actions, then push them
        //       the cost are deducted when performing the action so that
        //       if an actor gets impaired / slowed the action may fail in that cycle
        actor->ai.visit();
    }
    std::sort(
        actions_.begin(),
        actions_.end(),
        [](const auto& a, const auto& b) {
            return a->speed > b->speed;
        }
    );
    for (auto& action : actions_) {
        action->perform();
    }
    actions_.clear();

    update_fov(player_id_);
    player_action_ = nullptr;
}

void Scene::set_player(u64 id) {
    player_id_ = id;
}

Actor* Scene::player() {
    return get_actor_by_id(player_id_);
}

u64 Scene::player_id() {
    return player_id_;
}

bool Scene::on_key_down(KeyDownEvent& e) {
    Actor* player = Scene::player();
    if (!player) {
        return false;
    }
    auto move_relative = [=](Vec2<i32> direction) {
        player_action_ = &create_action<MoveAction>(player, player->position + direction);
    };
    switch (e.key) {
        case Key::W:
            move_relative(Vec2<i32> { 0, -1 });
            return true;
        case Key::A:
            move_relative(Vec2<i32> { -1, 0 });
            return true;
        case Key::S:
            move_relative(Vec2<i32> { 0, 1 });
            return true;
        case Key::D:
            move_relative(Vec2<i32> { 1, 0 });
            return true;
        default:
            return false;
    }
}