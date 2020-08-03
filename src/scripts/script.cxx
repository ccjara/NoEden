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

j_script::operator lua_State* () const noexcept {
    return state_;
}

bool j_script::run() {
    if (has_run_) {
        LOG(ERROR) << "Script " << id_ << " has already been called into";
        return false;
    }
    has_run_ = true;
    // TODO: improve
    switch (lua_pcall(state_, 0, 0, 0)) {
    case LUA_ERRRUN:
        LOG(ERROR) << "Runtime error in script " << id_ << ": " << lua_tostring(state_, lua_gettop(state_));
        return false;
    }
    return true;
}

void j_script::load() {
    has_run_ = false;
    if (state_) {
        lua_close(state_);
    }
    state_ = luaL_newstate();
    if (!state_) {
        LOG(ERROR) << "Could not allocate new lua state";
        status_ = j_script_status::error;
        return;
    }

    if (!path_.empty()) {
        if (!fs::exists(path_)) {
            LOG(ERROR) << "Cannot load script " << id_ << ": path " << path_ << " not found";
            status_ = j_script_status::error;
            return;
        }
        std::ifstream input { path_, std::ios::ate };
        if (input.bad()) {
            LOG(ERROR) << "Cannot load script " << id_ << ": bad input";
            status_ = j_script_status::error;
            return;
        }
        const auto size { input.tellg() };
        source_.resize(size);
        input.seekg(0);
        input.read(source_.data(), size);
    }

    const auto result { luaL_loadstring(state_, source_.c_str()) };
    if (result == LUA_OK) {
        status_ = j_script_status::loaded;
        LOG(INFO) << "Script " << id_ << " has been loaded";
    } else {
        LOG(ERROR) << "Cannot load script " << id_ << ": lua error result " << result << ": " << lua_tostring(state_, -1);

        status_ = j_script_status::error;
    }
}

j_script_status j_script::status() const noexcept {
    return status_;
}

bool j_script::loaded() const noexcept {
    return status_ == j_script_status::loaded;
}

const std::string& j_script::id() const noexcept {
    return id_;
}

lua_State* j_script::lua_state() const noexcept {
    return state_;
}

bool j_script::has_run() const noexcept {
    return has_run_;
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
    has_run_ = other.has_run_;
    path_ = std::move(other.path_);
    source_ = std::move(other.source_);

    return *this;
}
