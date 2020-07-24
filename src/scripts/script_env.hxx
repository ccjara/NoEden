#ifndef JARALYN_SCRIPT_ENV_HXX
#define JARALYN_SCRIPT_ENV_HXX

#include "script.hxx"
#include "../game_event.hxx"

class j_script_env {
    std::unordered_map<j_game_event_type, std::vector<luabridge::LuaRef>> listeners_;

    // item events
    void on_inventory_item_added(const j_inventory_item_added_event& e);

    /**
     * @brief Gets called when any lua function subscribes to any event
     */
    bool on_register_callback(const char* event_type, luabridge::LuaRef ref);
public:
    /**
     * @brief Attaches the scripting environment to game events
     */
    void attach(entt::dispatcher& dispatcher);

    /**
     * @brief Does a protected call into the given lua ref
     *
     * Checks whether the call was successful. In case of an error, the error
     * is logged, no exception is thrown.
     */
    template<typename... varg_t>
    constexpr inline void pcall_into(luabridge::LuaRef& ref, varg_t&&... args) const noexcept {
        const auto result { ref(std::forward<varg_t>(args)...) };
        if (result == std::nullopt) {
            const auto state { ref.state() };

            std::string err { "Caught error during script execution" };

            if (lua_gettop(state) > 0) {
                const char* e = lua_tostring(state, -1);
                if (e) {
                    err.append(": ");
                    err.append(e);
                }
            }
            LOG(ERROR) << err;
        }
    }

    /**
     * @brief Sets up the script environment for the given script
     */
    void setup(j_script& script);
};

#endif
