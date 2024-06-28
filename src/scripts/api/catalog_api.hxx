#ifndef NOEDEN_CATALOG_API_HXX
#define NOEDEN_CATALOG_API_HXX

#include "scripts/api/lua_api.hxx"

class AiNode;
class Archetype;
class Catalog;
class ServiceLocator;

class CatalogApi : public LuaApi {
public:
    void on_register(Script& script) override;

    void create_archetype(luabridge::LuaRef ref);

    void clear_archetypes();

    bool initialize() override;
private:
    Catalog* catalog_ = nullptr;

    std::unique_ptr<AiNode> create_behavior_node(const luabridge::LuaRef& ref);

    void add_behavior_component(Archetype& archetype, const luabridge::LuaRef& ref);
};

#endif
