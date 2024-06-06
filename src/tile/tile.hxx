#ifndef NOEDEN_TILE_HXX
#define NOEDEN_TILE_HXX

#include "../gfx/display_cell.hxx"

enum class TileType {
    Ground,
    Wall,
};

enum class MaterialType {
    None,
    Stone,
    Wood,
    Vegetation,
    Water,
};

enum class MaterialState {
    Void,
    Solid,
    Liquid,
};

enum TileFlags {
    /**
     * @brief No flags set
     */
    None,

    /**
     * @brief Whether the tile is solid and cannot be passed through
     */
    Blocking,

    /**
     * @brief Whether this tile has been within FoV by the player before
     */
    Revealed,

    /**
     * @brief Whether the tile is currently within the FoV of any player controlled entity
     */
    FoV,
};

struct Tile {
    TileType type = TileType::Ground;

    MaterialType material = MaterialType::None;

    MaterialState state = MaterialState::Void;

    std::bitset<8> flags;
    
    DisplayCell display_info;
};

#endif
