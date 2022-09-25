#include "fov.hxx"

void Fov::update(Entity& viewer, Grid<Tile>& tiles) {
    for (auto& t : tiles.cells()) {
        t.visited = false;
    }

    for (i32 q = 0; q < 4; ++q) {
        Row row(1, -1.0f, 1.0f);
        scan(viewer, tiles, row, (Quadrant) q);
    }
}

void Fov::scan(
    Entity& viewer,
    Grid<Tile>& tiles,
    Row& row,
    Quadrant q
) {
    Tile *prev_tile { nullptr };

    const i32 max_range = 9; // entity->view_range; TODO
    auto viewer_tile = tiles.at(viewer.position);
    if (viewer_tile) {
        viewer_tile->revealed = true;
        viewer_tile->visited = true;
    }

    for (i32 col = row.min_col; col <= row.max_col; ++col) {
        if (!in_range(row.depth, col, max_range)) {
            continue;
        }
        const auto grid_pos = to_grid_coords(q, viewer.position, row.depth, col);
        if (grid_pos.x < 0 || grid_pos.y < 0) {
            continue;
        }
        Tile* tile = tiles.at(grid_pos);
        if (!tile) {
            continue;
        }
        if (tile->solid || symmetric(row, col)) {
            tile->revealed = true;
            tile->visited = true;
        }
        if (prev_tile) {
            if (prev_tile->solid && !tile->solid) {
                row.set_start_slope(slope(row.depth, col));
            }
            if (!prev_tile->solid && tile->solid) {
                Row next = row.next();
                next.set_end_slope(slope(row.depth, col));
                scan(viewer, tiles, next, q);
            }
        }
        prev_tile = tile;
    }
    if (prev_tile && !prev_tile->solid && row.depth < max_range) {
        auto next = row.next();
        scan(viewer, tiles, next, q);
    }
}

float Fov::slope(i32 depth, i32 column) {
    return ((float) (2 * column - 1)) / ((float) 2 * depth);
}

bool Fov::symmetric(Row& row, i32 col) {
    return ((float) col >= (float) row.depth * row.start_slope) &&
            ((float) col <= (float) row.depth * row.end_slope);
}

bool Fov::in_range(i32 row, i32 col, i32 max_range) {
    return 0.5f + std::sqrt(col * col + row * row) <= max_range;
}

Vec2<i32> Fov::to_grid_coords(Quadrant q, Vec2<i32> start_pos, i32 depth, i32 col) {
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
}
