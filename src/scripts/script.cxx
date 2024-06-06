#include "script.hxx"

u64 Script::next_id_ { 0 };

Script::Script(std::string&& name, std::string&& source) :
    id(next_id_++),
    name_(std::move(name)),
    source_(std::move(source)) {
}

Script::~Script() {
    unload();
}

Script::operator lua_State* () const {
    return state_;
}

bool Script::run() {
    if (status_ == ScriptStatus::Unloaded) {
        Log::error("Cannot run script {}: script must be loaded", name_);
        return false;
    }
    if (lua_pcall(state_, 0, 0, 0) != LUA_OK) {
        fail(ScriptError::RuntimeError);
        return false;
    }
    status_ = ScriptStatus::Executed;
    return true;
}

void Script::load() {
    if (state_) {
        unload();
    }
    state_ = luaL_newstate();
    if (!state_) {
        return fail(ScriptError::StateAllocFailed);
    }
    if (luaL_loadstring(state_, source_.c_str()) != LUA_OK) {
        fail(ScriptError::ScriptCorrupted);
        unload();
        return;
    }
    luaL_openlibs(state_);
    status_ = ScriptStatus::Loaded;
}

void Script::unload() {
    if (state_) {
        {
            auto on_unload = luabridge::getGlobal(state_, "on_unload");
            if (on_unload.isFunction()) {
                pcall_into(on_unload);
            }
        }
        lua_close(state_);
        state_ = nullptr;
        status_ = ScriptStatus::Unloaded;
    }
    globals_.clear();
    callbacks_.clear();
}

ScriptStatus Script::status() const {
    return status_;
}

const std::string& Script::name() const {
    return name_;
}

lua_State* Script::lua_state() const {
    return state_;
}

const std::vector<std::string>& Script::globals() const {
    return globals_;
}

const std::string& Script::source() const {
    return source_;
}

void Script::set_source(std::string&& source) {
    source_ = std::move(source);
}

void Script::fail(ScriptError err) {
    switch (err) {
    case ScriptError::StateAllocFailed:
        Log::error("Could not allocate new lua state for script {}", name_);
        break;
    case ScriptError::RuntimeError:
        Log::error("Runtime error in script {}: {}", name_, lua_tostring(state_, -1));
        break;
    case ScriptError::ScriptCorrupted:
        Log::error("Cannot load script {}: ", name_, lua_tostring(state_, -1));
    case ScriptError::None:
    default:
        Log::error("Unknown error in script {}", name_);
    }
    error_ = err;
}
