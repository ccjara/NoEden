#ifndef JARALYN_SCRIPT_SYSTEM_HXX
#define JARALYN_SCRIPT_SYSTEM_HXX

#include "platform/platform_event.hxx"
#include "input/input_event.hxx"
#include "scripts/api/lua_api.hxx"
#include "scripts/script_event.hxx"
#include "scripts/script_util.hxx"
#include "scripts/script.hxx"
#include "scripts/script_loader.hxx"
#include "scripts/script_registry.hxx"

using lua_event_type = u32;
namespace lua_event { // FIXME: luabridge does not support enums
    static lua_event_type inventory_view{ 1000 };
}

class ScriptLoader;
class ScriptRegistry;

class Scripting {
public:
    explicit Scripting(EventManager* events);

    /**
     * @brief Constructs and registers a lua api fragment in place
     *
     * @see LuaApi
     */
    template<typename Api, typename... ApiArgs>
    void add_api(ApiArgs&& ... api_args);

    /**
     * @brief Setups up globals and namespaces for every script.
     */
    void setup_script_env(Script& script);

    /**
     * @brief Reloads all scripts
     */
    void reload();

    const std::unordered_map<u64, std::unique_ptr<Script>>& scripts() const;
    std::unordered_map<u64, std::unique_ptr<Script>>& scripts();
    Script* get_by_id(u64 id);

    ~Scripting();
    Scripting(const Scripting&) = delete;
    Scripting& operator=(const Scripting&) = delete;
    Scripting(Scripting&&) = delete;
    Scripting& operator=(Scripting&&) = delete;
private:
    std::unique_ptr<ScriptLoader> script_loader_ = nullptr;
    std::unique_ptr<ScriptRegistry> script_registry_ = nullptr;
    std::vector<std::unique_ptr<LuaApi>> apis_;

    EventManager* events_ = nullptr;

    struct ScriptRef {
        u64 script_id;
        luabridge::LuaRef ref;
    };
    std::unordered_map<lua_event_type, std::vector<ScriptRef>> listeners_;

    /**
     * @brief Tracks a lua function that should be invoked on a specific event.
     *
     * The stored refs must be removed before the scripts are unloaded
     */
    bool register_lua_callback(lua_event_type event_type, luabridge::LuaRef ref);

    bool on_key_down(KeyDownEvent& e);

    /**
     * @brief Calls all registered lua callbacks for a specific event type.
     *
     * The arguments are forwarded to each callback.
     */
    template<typename... args>
    void invoke_lua_callbacks(lua_event_type event_type, args&& ... arguments) {
        const auto it{ listeners_.find(event_type) };
        if (it == listeners_.end()) {
            return;
        }
        for (auto& script_ref: it->second) {
            pcall_into(script_ref.ref, std::forward<args>(arguments)...);
        }
    }

    /**
     * @brief Loads a script after collecting all scripts in {@see load_from_path}
     */
    void load(Script& script);

    /**
     * @brief Resets the script system to its initial state
     *
     * In this state, no script or any reference thereof is held
     */
    void reset();

    static constexpr const char* default_script_path{
#ifdef NDEBUG
        "scripts"
#else
        "../src/scripts/lua"
#endif
    };
};

template<typename Api, typename... ApiArgs>
void Scripting::add_api(ApiArgs&& ... api_args) {
    apis_.emplace_back(new Api(std::forward<ApiArgs>(api_args)...));
}

#endif
