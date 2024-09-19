#ifndef NOEDEN_WORLD_SPEC_HXX
#define NOEDEN_WORLD_SPEC_HXX

#include "framework/noise_generator.hxx"

class WorldSpec {
    friend class WorldSpecCreator;
public:
    i32 version() const;

    i32 seed() const;

    i32 chunks_x() const;

    i32 chunks_z() const;

    i32 max_vegetation() const;

    i32 max_shoreline() const;

    i32 max_water() const;

    /**
     * @brief Returns the height map value at the given chunk position
     */
    float height_at(const ChunkPos& chunk_pos) const;
private:
    /**
     * @brief Width of the world in chunks
     */
    i32 chunks_x_;

    /**
     * @brief Height of the world in chunks
     */
    i32 chunks_z_;

    /**
     * @brief Seed used to generate the world and its contents
     */
    i32 seed_;

    /**
     * @brief Version of the world data
     */
    i32 version_;
};

#endif
