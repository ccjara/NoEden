#include "script.hxx"

j_script::j_script(const std::string& name) : name_(name) {
}

j_script::~j_script() {
    unload();
}

j_script::operator lua_State* () const {
    return state_;
}

bool j_script::run() {
    if (status_ == j_script_status::unloaded) {
        LOG(ERROR) << "Cannot run script " << name_ << ": script must be loaded";
        return false;
    }
    if (lua_pcall(state_, 0, 0, 0) != LUA_OK) {
        fail(j_script_error::runtime_error);
        return false;
    }
    status_ = j_script_status::executed;
    return true;
}

void j_script::load() {
    if (state_) {
        unload();
    }
    state_ = luaL_newstate();
    if (!state_) {
        return fail(j_script_error::state_alloc_failed);
    }
    if (luaL_loadstring(state_, source_.c_str()) != LUA_OK) {
        unload();
        return fail(j_script_error::script_corrupted);
    }
    luaL_openlibs(state_);
    status_ = j_script_status::loaded;
}

void j_script::unload() {
    if (state_) {
        lua_close(state_);
        state_ = nullptr;
        status_ = j_script_status::unloaded;
    }
    globals_.clear();
}

j_script_status j_script::status() const {
    return status_;
}

const std::string& j_script::name() const {
    return name_;
}

lua_State* j_script::lua_state() const {
    return state_;
}

const std::vector<std::string>& j_script::globals() const {
    return globals_;
}

const std::string& j_script::source() const {
    return source_;
}

void j_script::set_source(std::string&& source) {
    source_ = std::move(source);
}

j_script::j_script(j_script&& other) {
    *this = std::move(other);
}

j_script& j_script::operator=(j_script&& other) {
    name_ = std::move(other.name_);
    if (other.state_ != nullptr) {
        state_ = std::exchange(other.state_, nullptr);
    }
    status_ = other.status_;
    error_ = other.error_;
    path_ = std::move(other.path_);
    source_ = std::move(other.source_);

    return *this;
}

void j_script::fail(j_script_error err) {
    switch (err) {
    case j_script_error::state_alloc_failed:
        LOG(ERROR) << "Could not allocate new lua state";
        break;
    case j_script_error::runtime_error:
        assert(lua_gettop(state_) == -1);
        LOG(ERROR) << "Runtime error in script " << name_ << ": " << lua_tostring(state_, -1);
        break;
    case j_script_error::script_path_not_found:
        LOG(ERROR) << "Cannot load script " << name_ << ": path " << path_ << " not found";
        break;
    case j_script_error::bad_script_input:
        LOG(ERROR) << "Cannot load script " << name_ << ": bad input";
        break;
    case j_script_error::script_corrupted:
        LOG(ERROR) << "Cannot load script " << name_ << ": " << lua_tostring(state_, -1);
    case j_script_error::none:
        return;
    default:
        LOG(ERROR) << "Unknown error in script " << name_;
    }
    error_ = err;
}
