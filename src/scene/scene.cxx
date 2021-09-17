#include "scene.hxx"

Scene::Scene() : tiles_ { TileBuilder::wall() } {
    tiles_.resize({ 120, 40 });

    for (auto& t : tiles_.cells()) {
        t = TileBuilder::floor();
    }

    Log::debug("Tiles are {}x{}", tiles_.dimensions().x, tiles_.dimensions().y);

    *tiles_.at(Vec2<u32>{ 5, 5 }) = TileBuilder::wall();
    *tiles_.at(Vec2<u32>{ 6, 4 }) = TileBuilder::wall();
    *tiles_.at(Vec2<u32>{ 6, 5 }) = TileBuilder::wall();
    *tiles_.at(Vec2<u32>{ 6, 6 }) = TileBuilder::wall();
    *tiles_.at(Vec2<u32>{ 7, 5 }) = TileBuilder::wall();
    *tiles_.at(Vec2<u32>{ 8, 4 }) = TileBuilder::wall();
    *tiles_.at(Vec2<u32>{ 9, 6 }) = TileBuilder::wall();
    *tiles_.at(Vec2<u32>{ 7, 7 }) = TileBuilder::wall();
    *tiles_.at(Vec2<u32>{ 4, 3 }) = TileBuilder::wall();
    *tiles_.at(Vec2<u32>{ 5, 2 }) = TileBuilder::wall();
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

void Scene::update_pov(Actor* player) {
    assert(player);

    Fov::update(*player, tiles_);
}
