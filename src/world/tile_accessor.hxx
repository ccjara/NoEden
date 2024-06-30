#ifndef NOEDEN_TILE_ACCESSOR_HXX
#define NOEDEN_TILE_ACCESSOR_HXX

class ChunkManager;
struct WorldContext;
struct Tile;

class TileAccessor {
public:
    void initialize(WorldContext* world_context);

    Tile* get_tile(const WorldPos& position);
    void set_tile(const WorldPos& position, const Tile& tile);

    size_t to_local_index(WorldPos position) const;
private:
    ChunkManager* chunk_manager_ = nullptr;
    WorldContext* world_context_ = nullptr;
};

#endif
