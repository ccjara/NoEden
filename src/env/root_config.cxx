#include "root_config.hxx"

j_root_config::j_root_config() : 
    resolution_ { 800, 600 }
    {
}

j_root_config::j_root_config(j_script& sys_script) : j_root_config() {
    namespace lua = luabridge;

    constexpr const auto fail = [](std::string_view reason) noexcept -> void {
        LOG(ERROR) << "Cannot load root config: " << reason << ". Loading fallback configuration.";
    };

    if (!sys_script.loaded()) {
        fail("System script is not loaded");
        return;
    }
    sys_script.run();

    const auto window_cfg = lua::getGlobal(sys_script, "window");
    if (!window_cfg.isTable()) {
        fail("Corrupted system script. Expected 'window' global to be a table");
        return;
    }
    // TODO: abstract runtime type checking and default value handling
    if (window_cfg["width"].isNumber()) {
        resolution_.width = window_cfg["width"];
    }
    if (window_cfg["height"].isNumber()) {
        resolution_.height = window_cfg["height"];
    }
}