#ifndef JARALYN_CATALOG_API_HXX
#define JARALYN_CATALOG_API_HXX

#include "lua_api.hxx"
#include "../../ai/ai_closest_entity.hxx"
#include "../../ai/ai_selector.hxx"
#include "../../ai/ai_walk.hxx"
#include "../../entity/catalog.hxx"
#include "../../entity/components/render.hxx"
#include "../../entity/components/behavior.hxx"
#include "../../entity/components/vision.hxx"

class CatalogApi final : public LuaApi {
public:
    void on_register(Script* script) final override;

    void create_archetype(luabridge::LuaRef ref);

    void clear_archetypes();
private:
    void add_behavior_component(Archetype& archetype, const luabridge::LuaRef& ref);
};

#endif
