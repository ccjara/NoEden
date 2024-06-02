#include "tile_manager.hxx"
#include "tile_factory.hxx"

TileManager::TileManager() {
    tiles_.resize({ 120, 40 });

    std::srand(std::time(0));

    for (auto& t : tiles_.cells()) {
        t = TileFactory::grass();
    }

    for (int i=0; i<100; ++i) {
        auto x = std::rand() % 120;
        auto y = std::rand() % 40;

        tiles_.put(TileFactory::wall(), Vec2<i32> { x, y });
    }

    for (int i=0; i<100; ++i) {
        auto x = std::rand() % 120;
        auto y = std::rand() % 40;

        tiles_.put(TileFactory::water(), Vec2<i32> { x, y });
    }
}

Tile* TileManager::tile(Vec2<i32> position) {
    return tiles_.at(position);
}


const Tile* TileManager::tile(Vec2<i32> position) const {
    return tiles_.at(position);
}

Grid<Tile> const& TileManager::tiles() const {
    return tiles_;
}

Grid<Tile>& TileManager::tiles() {
    return tiles_;
}

Tile& TileManager::create_tile(Vec2<i32> position) {
    auto t = TileFactory::none();
    tiles_.put(t, position);
    return *tiles_.at(position);
}

