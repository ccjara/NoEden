#pragma once

#include "scripts/api/lua_api.hxx"

class Catalog;

class CatalogApi : public LuaApi {
public:
    void on_register(Script& script) override;

    bool initialize() override;
private:
    Catalog* catalog_ = nullptr;
};
