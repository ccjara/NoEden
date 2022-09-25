#ifndef JARALYN_CATALOG_API_HXX
#define JARALYN_CATALOG_API_HXX

#include "lua_api.hxx"
#include "../../actor/catalog.hxx"
#include "../../actor/components/render.hxx"

class CatalogApi final : public LuaApi {
public:
    void on_register(Script* script) final override;

    void create_archetype(luabridge::LuaRef ref);
private:
};

#endif
