#include "scene.hxx"

Scene::Scene() : tiles_ { TileBuilder::wall() } {
    tiles_.resize({ 120, 40 });
    for (auto& t : tiles_.cells()) {
        t = TileBuilder::floor();
    }
}

Actor* Scene::get_actor_by_id(u64 id) const {
    auto it { actors_by_id_.find(id) };
    return it == actors_by_id_.end() ? nullptr : it->second;
}

Actor& Scene::create_actor(const Archetype* archetype) {
    auto& actor { actors_.emplace_back(new Actor(archetype)) };
    actors_by_id_[actor->id] = actor.get();
    return *actor;
}

Scene::ActorContainer& Scene::actors() {
    return actors_;
}

const Scene::ActorContainer& Scene::read_actors() const {
    return actors_;
}

Grid<Tile>& Scene::tiles() {
    return tiles_;
}

const Grid<Tile>& Scene::read_tiles() const {
    return tiles_;
}

void Scene::update_fov(Actor* player) {
    if (player) {
        Fov::update(*player, tiles_);
    }
}
