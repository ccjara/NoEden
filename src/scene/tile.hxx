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

    /**
     * @brief Whether the tile is currently within the FoV of the player
     */
    bool fov { false };

    /**
     * @brief Whether this tile has been within FoV by the player before
     */
    bool revealed { false };

    DisplayCell display_info;
};

#endif
