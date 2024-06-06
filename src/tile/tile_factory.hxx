#ifndef NOEDEN_TILE_FACTORY_HXX
#define NOEDEN_TILE_FACTORY_HXX

#include "tile.hxx"

class TileFactory {
public:
    static Tile none();
    static Tile wall();
    static Tile grass();
    static Tile water();
};

#endif
