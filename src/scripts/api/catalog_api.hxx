#ifndef NOEDEN_CATALOG_API_HXX
#define NOEDEN_CATALOG_API_HXX

#include "scripts/api/lua_api.hxx"

class AiNode;
struct  Archetype;
class Catalog;
class ConditionResolver;

class CatalogApi : public LuaApi {
public:
    void on_register(Script& script) override;

    void create_archetype(const luabridge::LuaRef& ref);

    void clear_archetypes();

    bool initialize() override;
private:
    Catalog* catalog_ = nullptr;

    std::unique_ptr<AiNode> create_behavior_node(const luabridge::LuaRef& ref);

    ConditionResolver* condition_resolver_ = nullptr;

    void add_behavior_component(Archetype& archetype, const luabridge::LuaRef& ref);
};

#endif
