#ifndef JARALYN_SCENE_API_HXX
#define JARALYN_SCENE_API_HXX

#include "lua_api.hxx"
#include "../../scene/scene.hxx"

class SceneApi final : public LuaApi {
public:
    void on_register(Script* script) final override;

    /**
     * @brief Returns the Entity name for the given entity id.
     *
     * Returns nullptr if the Entity does not exist.
     */
    const char* entity_name(Id id) const;

    /**
     * @brief Shorthand helper which combines player_id() and entity_name()
     *
     * Returns nullptr if no Entity is currently designated as the player.
     */
    const char* player_name() const;

    /**
     * @brief Returns the id of the Entity currently designated as the player.
     *
     * Returns null_id if no player currently exists
     */
    Id player_id() const;
};

#endif
