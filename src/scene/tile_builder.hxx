#ifndef JARALYN_TILE_BUILDER_HXX
#define JARALYN_TILE_BUILDER_HXX

#include "tile.hxx"

class TileBuilder {
public:
    [[nodiscard]] static Tile none();
    [[nodiscard]] static Tile wall();
    [[nodiscard]] static Tile floor();
    [[nodiscard]] static Tile water();

    [[nodiscard]] static Tile for_type(TileType type);
};

#endif
