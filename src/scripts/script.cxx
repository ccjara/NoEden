#include "script.hxx"

j_script::j_script(const std::string& id, const fs::path& path) :
    id_(id),
    path_(path.string().c_str()) {
}

j_script::j_script(const std::string& id, const std::string& source) :
    id_(id),
    source_(source) {
}

j_script::~j_script() {
    if (state_) {
        lua_close(state_);
    }
}

j_script::operator lua_State* () const {
    return state_;
}

bool j_script::run() {
    if (status_ != j_script_status::loaded) {
        LOG(ERROR) << "Cannot run script " << id_ << ": script must be loaded";
        return false;
    }
    if (lua_pcall(state_, 0, 0, 0) != LUA_OK) {
        fail(j_script_error::runtime_error);
        return false;
    }
    status_ = j_script_status::called;
    return true;
}

void j_script::load() {
    // realloc / reset script state if reloading
    if (state_) {
        lua_close(state_);
    }
    state_ = luaL_newstate();
    if (!state_) {
        return fail(j_script_error::state_alloc_failed);
    }
    // load source from file if path is given
    if (!path_.empty()) {
        if (!fs::exists(path_)) {
            return fail(j_script_error::script_path_not_found);
        }
        std::ifstream input { path_, std::ios::ate };
        if (input.bad()) {
            return fail(j_script_error::bad_script_input);
        }
        const auto size { input.tellg() };
        source_.resize(size);
        input.seekg(0);
        input.read(source_.data(), size);
    }
    if (luaL_loadstring(state_, source_.c_str()) != LUA_OK) {
        return fail(j_script_error::script_corrupted);
    }
    LOG(INFO) << "Script " << id_ << " has been loaded";
    status_ = j_script_status::loaded;
    luaL_openlibs(state_);
}

j_script_status j_script::status() const {
    return status_;
}

bool j_script::callable() const {
    return status_ == j_script_status::loaded;
}

bool j_script::called() const {
    return status_ == j_script_status::called;
}

bool j_script::loaded() const {
    return callable();
}

const std::string& j_script::id() const {
    return id_;
}

lua_State* j_script::lua_state() const {
    return state_;
}

j_script::j_script(j_script&& other) {
    *this = std::move(other);
}

j_script& j_script::operator=(j_script&& other) {
    id_ = std::move(other.id_);
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
        LOG(ERROR) << "Runtime error in script " << id_ << ": " << lua_tostring(state_, -1);
        break;
    case j_script_error::script_path_not_found:
        LOG(ERROR) << "Cannot load script " << id_ << ": path " << path_ << " not found";
        break;
    case j_script_error::bad_script_input:
        LOG(ERROR) << "Cannot load script " << id_ << ": bad input";
        break;
    case j_script_error::script_corrupted:
        LOG(ERROR) << "Cannot load script " << id_ << ": " << lua_tostring(state_, -1);
    case j_script_error::none:
        return;
    default:
        LOG(ERROR) << "Unknown error in script " << id_;
    }
    status_ = j_script_status::error;
    error_ = err;
}
