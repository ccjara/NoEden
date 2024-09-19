#ifndef NOEDEN_CHUNK_HXX
#define NOEDEN_CHUNK_HXX

#include "world/tile.hxx"

struct Chunk {
    static constexpr i32 CHUNK_SIDE_LENGTH = 128;
    static constexpr i32 CHUNK_DEPTH = 1;
    static constexpr i32 MAX_TILES = CHUNK_SIDE_LENGTH * CHUNK_SIDE_LENGTH * CHUNK_DEPTH;
    static constexpr i32 MAX_INDEX = MAX_TILES - 1;

    std::array<Tile, MAX_TILES> tiles;

    inline Tile& tile(i32 x, i32 y, i32 z) {
        assert(x >= 0 && x < CHUNK_SIDE_LENGTH);
        assert(y >= 0 && y < CHUNK_DEPTH);
        assert(z >= 0 && z < CHUNK_SIDE_LENGTH);

        return tiles[x + z * CHUNK_SIDE_LENGTH + y * CHUNK_SIDE_LENGTH * CHUNK_SIDE_LENGTH];
    }
};

#endif
