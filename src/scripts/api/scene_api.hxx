#ifndef JARALYN_SCENE_API_HXX
#define JARALYN_SCENE_API_HXX

#include "lua_api.hxx"
#include "../../scene/scene.hxx"

class SceneApi final : public LuaApi {
public:
    void on_register(Script* script) final override;

    /**
     * @brief Returns the Actor name for the given entity id.
     *
     * Returns nullptr if the Actor does not exist.
     */
    const char* actor_name(Id id) const;

    /**
     * @brief Shorthand helper which combines player_id() and actor_name()
     *
     * Returns nullptr if no Actor is currently designated as the player.
     */
    const char* player_name() const;

    /**
     * @brief Returns the id of the Actor currently designated as the player.
     *
     * Returns null_id if no player currently exists
     */
    Id player_id() const;
};

#endif
