#ifndef JARALYN_CATALOG_API_HXX
#define JARALYN_CATALOG_API_HXX

#include "lua_api.hxx"
#include "../../entity/catalog.hxx"
#include "../../entity/components/render.hxx"

class CatalogApi final : public LuaApi {
public:
    void on_register(Script* script) final override;

    void create_archetype(luabridge::LuaRef ref);

    void clear_archetypes();
private:
};

#endif
