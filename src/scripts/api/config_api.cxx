#include "scripts/api/config_api.hxx"
#include "config/config_manager.hxx"

bool ConfigApi::initialize() {
    config_manager_ = svc_->get<ConfigManager>();
    if (!config_manager_) {
        LOG_ERROR("ConfigApi failed to initialize: failed to get ConfigManager");
        return false;
    }

    return true;
}

void ConfigApi::on_register(Script& script) {
    luabridge::getGlobalNamespace(script)
        .beginClass<ConfigApi>("ConfigApi")
            .addFunction("configure", &ConfigApi::on_configure)
        .endClass();

    expose(script, this, "config");
}

void ConfigApi::on_configure(luabridge::LuaRef cfg) {
    Config new_config;

    constexpr auto report = [](std::string_view reason) -> void {
        LOG_ERROR("Error in root config: {}", reason);
    };

    if (!cfg.isTable()) {
        report("Expected payload to be a table");
        return;
    }

    // TODO: abstract runtime type checking and default value handling
    if (cfg["scaling"].isNumber()) {
        new_config.scaling = cfg["scaling"].cast<u32>().value();
    } else {
        report("Expected gfx:scaling to be a number");
    }
    if (cfg["font_texture_path"].isString()) {
        new_config.font_texture_path = cfg["font_texture_path"].cast<std::string>().value();
    } else {
        report("Expected gfx:font_texture_path to be a string");
    }

    const auto& glyph_size { cfg["glyph_size"] };
    if (glyph_size.isTable()) {
        if (glyph_size["width"].isNumber()) {
            new_config.glyph_size.x = glyph_size["width"];
        } else {
            report("Expected gfx:glyph_size:width to be a number");
        }
        if (glyph_size["height"].isNumber()) {
            new_config.glyph_size.y = glyph_size["height"];
        } else {
            report("Expected gfx:glyph_size:height to be a number");
        }
    } else {
        report("Expected gfx:glyph_size to be a table");
    }

    config_manager_->update(std::move(new_config));
}
