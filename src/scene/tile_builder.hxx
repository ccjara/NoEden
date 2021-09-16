#ifndef JARALYN_TILE_BUILDER_HXX
#define JARALYN_TILE_BUILDER_HXX

#include "tile.hxx"

class TileBuilder {
public:
    [[nodiscard]] static Tile wall();
    [[nodiscard]] static Tile floor();
};

#endif
