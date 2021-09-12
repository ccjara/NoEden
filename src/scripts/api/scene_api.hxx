#ifndef JARALYN_SCENE_API_HXX
#define JARALYN_SCENE_API_HXX

#include "lua_api.hxx"
#include "../../scene/scene.hxx"

/*
FIX CRASH ON EXIT
Improve lua error handling (probably a rare opportunity to use a global static for this one)
    that does nothing but dispatch a LuaErrorEvent so the xray can pick it up
*/

class SceneApi final : public LuaApi {
public:
    SceneApi(Scene& scene);

    void on_register(Script* script) final override;

    /**
     * @brief Returns the actor name for the given entity id.
     * 
     * Returns "Unknown" if the actor does not exist.
     */
    const char* actor_name(u64 id) const;
private:
    Scene& scene_;
};

#endif
