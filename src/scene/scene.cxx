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
    for (auto& t : tiles_.cells()) {
        t.visited = false;
    }

    const Vec2<i32> start_pos = player->position;
    i32 MAX_DEPTH = 10;

    // based on https://www.albertford.com/shadowcasting

    struct Row {
        i32 depth;
        float start_slope;
        float end_slope;
        i32 min_col;
        i32 max_col;

        Row(i32 depth, float start_slope, float end_slope) :
            depth(depth),
            start_slope(start_slope),
            end_slope(end_slope) {

            const auto min_raw = ((float) depth) * start_slope;
            const auto max_raw = ((float) depth) * end_slope;

            min_col = std::floor((float) min_raw + 0.5f); // TODO: needed?
            max_col = std::ceil((float) max_raw - 0.5f); // TODO: needed?
        }

        Row next() {
            return Row(depth + 1, start_slope, end_slope);
        }
    };

    enum class Quadrant { N, E, S, W };

    const auto translate = [=](Quadrant q, i32 depth, i32 col) -> Vec2<i32> {
        switch (q) {
            case Quadrant::N:
            default:
                return { start_pos.x + col, start_pos.y - depth };
            case Quadrant::S:
                return { start_pos.x + col, start_pos.y + depth };
            case Quadrant::E:
                return { start_pos.x + depth, start_pos.y + col };
            case Quadrant::W:
                return { start_pos.x - depth, start_pos.y + col };
        }
    };

    constexpr auto symmetric = [](Row& row, i32 col) -> bool {
        return (col >= row.depth * row.start_slope) &&
            (col <= row.depth * row.end_slope);
    };

    constexpr auto slope = [](i32 depth, i32 column) -> float {
        return ((float) (2 * column - 1)) / ((float) 2 * depth);
    };

    for (i32 q = 0; q < 4; ++q) {
        std::vector<Row> rows;
        rows.emplace_back(1, -1.0f, 1.0f);

        while (!rows.empty()) {
            Row row = rows.back();
            rows.pop_back();

            Tile *prev_tile { nullptr };

            for (i32 col = row.min_col; col <= row.max_col; ++col) {
                const auto grid_raw = translate((Quadrant) q, row.depth, col);
                if (grid_raw.y < 0) {
                    continue;
                }
                if (grid_raw.x < 0) {
                    continue;
                }
                const Vec2<u32> grid_pos { (u32) grid_raw.x, (u32) grid_raw.y };

                Tile* tile = tiles_.at(grid_pos);
                if (tile->solid || symmetric(row, col)) {
                    tile->revealed = true;
                    tile->visited = true;
                }
                if (prev_tile) {
                    if (prev_tile->solid && !tile->solid) {
                        row.start_slope = slope(row.depth, col);
                    }
                    if (!prev_tile->solid && tile->solid) {
                        Row next = row.next();
                        next.end_slope = slope(row.depth, col);
                        rows.push_back(next);
                    }
                }
                prev_tile = tile;
            }
            if (prev_tile && !prev_tile->solid && row.depth < MAX_DEPTH) {
                rows.push_back(row.next());
            }
        }
    }
}
