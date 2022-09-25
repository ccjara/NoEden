#include "catalog_api.hxx"

void CatalogApi::on_register(Script* script) {
    luabridge::getGlobalNamespace(*script)
        .beginClass<CatalogApi>("Catalog")
            .addFunction("create_archetype", &CatalogApi::create_archetype)
        .endClass();

    expose(script, this, "catalog");
}

void CatalogApi::create_archetype(luabridge::LuaRef ref) {
    if (!ref.isTable()) {
        Log::error("Expected archetype data to be a table");
        return;
    }
    if (!ref["name"].isString()) {
        Log::error("Archetype specification has no name");
        return;
    }
    auto archetype = Catalog::create_archetype(ref["name"].cast<const char*>());
    if (!archetype) {
        return;
    }
    const auto speed_ref = ref["speed"];
    if (speed_ref.isNumber()) {
        archetype->speed = speed_ref.cast<i32>();
    }
    const auto components_ref = ref["components"];
    if (components_ref.isTable()) {
        const auto length = components_ref.length();

        auto parse_components = [archetype](const luabridge::LuaRef& r) -> void {
            if (r.isNil()) {
                return;
            }
            const auto type_ref = r["type"];
            if (!type_ref.isNumber()) {
                Log::error("Invalid {} component: type must be a string", archetype->name);
            }
            const auto component_type_unsafe = type_ref.cast<i32>();
            switch (static_cast<ComponentType>(component_type_unsafe)) {
                case ComponentType::Render: {
                    const auto glyph_ref = r["glyph"];
                    auto component_ptr = new Render();
                    archetype->components.emplace_back(component_ptr);
                    auto& info = component_ptr->display_info();
                    if (!glyph_ref.isNumber()) {
                        Log::warn("Glyph in render component spec of {} is not a number", archetype->name);
                        return;
                    }
                    info.glyph = glyph_ref.cast<i32>();
                    info.visible = true;
                    info.color = Color::white(); // TODO -> lua
                    return;
                }
                default:
                    Log::warn("Invalid {} component: unknown type id {}", archetype->name, component_type_unsafe);
            }
        };

        for (i32 nz_offset = 1U; nz_offset <= length; ++nz_offset) {
            parse_components(components_ref[nz_offset]);
        }
    }
}
