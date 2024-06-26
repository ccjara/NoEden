#ifndef NOEDEN_WORLD_SPEC_CREATOR_HXX
#define NOEDEN_WORLD_SPEC_CREATOR_HXX

class WorldSpec;

struct CreateWorldSpecOptions {
    /**
     * @brief Seed used to generate the world and its contents
     */
    i32 seed = 0;

    /**
     * @brief Width of the map in chunks
     */
    i32 chunks_x = 0;

    /**
     * @brief Height of the map in chunks
     */
    i32 chunks_z = 0;

    /**
     * @brief Max elevation where vegetation is encountered
     */
    i32 max_vegetation = 0;

    /**
     * @brief Max elevation where shorelines are encountered
     */
    i32 max_shoreline = 0;

    /**
     * @brief Max elevation where seawater is encountered
     */
    i32 max_water = 0;
};

class WorldSpecCreator {
public:
    void generate_height_map(WorldSpec& spec);
    void generate_humidity_map(WorldSpec& spec);

    [[nodiscard]] std::unique_ptr<WorldSpec> create_world_spec(const CreateWorldSpecOptions& options);
};

#endif
