#include "scripts/api/catalog_api.hxx"
#include "catalog/catalog.hxx"
#include "resource/resource_manager.hxx"

bool CatalogApi::initialize() {
    catalog_ = svc_->get<ResourceManager>()->catalog();
    if (!catalog_) {
        LOG_ERROR("CatalogApi failed to initialize: failed to get Catalog");
        return false;
    }
    return true;
}

void CatalogApi::on_register(Script& script) {
    luabridge::getGlobalNamespace(script)
        .beginClass<CatalogApi>("Catalog")
        .endClass();

    expose(script, this, "catalog");
}
