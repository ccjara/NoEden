#include "script.hxx"

u64 Script::next_id_ = 0;

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

bool Script::call() {
    if (status_ == ScriptStatus::Unloaded) {
        LOG_ERROR("Cannot run script {}: script must be loaded", name_);
        return false;
    }
    if (lua_pcall(state_, 0, 0, 0) != LUA_OK) {
        fail(ScriptError::RuntimeError);
        return false;
    }
    status_ = ScriptStatus::Called;
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
    globals_.clear();
    callbacks_.clear();
    if (state_) {
        lua_close(state_);
        state_ = nullptr;
        status_ = ScriptStatus::Unloaded;
    }
}

ScriptStatus Script::status() const {
    return status_;
}

ScriptError Script::error() const {
    return error_;
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
        LOG_ERROR("Could not allocate new lua state for script {}", name_);
        break;
    case ScriptError::RuntimeError:
        LOG_ERROR("Runtime error in script {}: {}", name_, lua_tostring(state_, -1));
        break;
    case ScriptError::ScriptCorrupted:
        LOG_ERROR("Cannot load script {}: ", name_, lua_tostring(state_, -1));
    case ScriptError::None:
    default:
        LOG_ERROR("Unknown error in script {}", name_);
    }
    error_ = err;
}
