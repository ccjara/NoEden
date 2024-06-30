#ifndef NOEDEN_SCRIPT_SYSTEM_HXX
#define NOEDEN_SCRIPT_SYSTEM_HXX

#include "platform/platform_event.hxx"
#include "scripts/api/lua_api.hxx"
#include "scripts/script.hxx"
#include "scripts/script_event.hxx"

class ScriptLoader;
class ScriptRegistry;

struct LuaCallback {
    Script* script = nullptr;

    i32 ref = 0;
};

class Scripting {
public:
    Scripting(ServiceLocator* services, EventManager* events);

    /**
     * @brief Constructs and registers a lua api fragment in place
     *
     * @see LuaApi
     */
    template <typename Api, typename... ApiArgs>
    void add_api(ApiArgs&&... api_args);

    /**
     * @brief Reloads all scripts
     */
    void reload();

    /**
     * @brief Runs a script by name
     *
     * @return True if the script was found and executed
     */
    bool run(std::string_view name);

    void add_callback(EventType event_type, luabridge::LuaRef& callback);

    const std::vector<LuaCallback>& callbacks_by_event_type(EventType event_type) const;
    const std::vector<LuaCallback>& callbacks_by_script_id(Id script_id) const;

    const std::unordered_map<u64, std::unique_ptr<Script>>& scripts() const;
    std::unordered_map<u64, std::unique_ptr<Script>>& scripts();
    Script* get_by_id(u64 id);
    Script* get_by_lua_ref(luabridge::LuaRef& ref);

    ~Scripting();
    Scripting(const Scripting&) = delete;
    Scripting& operator=(const Scripting&) = delete;
    Scripting(Scripting&&) = delete;
    Scripting& operator=(Scripting&&) = delete;

private:
    bool run(Script& script);

    std::unique_ptr<ScriptLoader> script_loader_ = nullptr;
    std::unique_ptr<ScriptRegistry> script_registry_ = nullptr;
    std::vector<std::unique_ptr<LuaApi>> apis_;

    EventManager* events_ = nullptr;
    ServiceLocator* svc_ = nullptr;

    EventResult on_key_down(KeyDownEvent& e);
    Subscription<KeyDownEvent> key_down_sub_;

    std::unordered_map<Id, std::vector<LuaCallback>> callbacks_by_script_id_ = {};
    std::unordered_map<EventType, std::vector<LuaCallback>> callbacks_by_event_type_ = {};
    std::vector<LuaCallback> empty_callbacks_ = {};

    void clear_callbacks(Id script_id);

    /**
     * @brief Loads a script
     *
     * @param script Script to load
     */
    void load(Script& script);

    /**
     * @brief Sets up the script environment
     *
     * @param script Script to set up
     */
    void setup_script_env(Script& script);

    /**
     * @brief Unloads a script
     *
     * @param script Script to unload
     */
    void unload(Script& script);

    static constexpr const char* default_script_path{
#ifdef NDEBUG
        "scripts"
#else
        "../src/scripts/lua"
#endif
    };
};

template <typename Api, typename... ApiArgs>
void Scripting::add_api(ApiArgs&&... api_args) {
    auto api = std::make_unique<Api>(std::forward<ApiArgs>(api_args)...);

    api->svc_ = svc_;
    api->events_ = events_;

    if (!api->initialize()) {
        return;
    }

    apis_.push_back(std::move(api));
}

#endif
