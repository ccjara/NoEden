#ifndef JARALYN_TILE_WRITER_HXX
#define JARALYN_TILE_WRITER_HXX

struct Tile;

class ITileWriter {
public:
    /**
     * @brief Creates a tile at the given position
     * 
     * @param position Position of the tile to create
     * 
     * @return Tile& Reference to the created tile for further configuration
     */
    virtual Tile& create_tile(Vec2<i32> position) = 0;

    virtual ~ITileWriter() = default;
};

#endif