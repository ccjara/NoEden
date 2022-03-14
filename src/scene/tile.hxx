#ifndef JARALYN_TILE_HXX
#define JARALYN_TILE_HXX

#include "../gfx/display_cell.hxx"

enum class TileType {
    None,
    Floor,
    Wall,
    Water,
};

struct Tile {
    TileType type { TileType::Wall };
    bool solid { true };
    bool visited { false };
    bool revealed { false };
    DisplayCell display_info;
};

#endif
