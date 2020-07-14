#include "script.hxx"

j_script::j_script(const std::string& id, const fs::path& path)
    : id_(id) {
    state_ = luaL_newstate();

    if (!state_) {
        LOG(ERROR) << "Could not allocate new lua state";
        status_ = j_script_status::error;
        return;
    }
    const auto result { luaL_loadfilex(state_, path.string().c_str(), nullptr) };

    if (result == LUA_OK) {
        status_ = j_script_status::loaded;
    } else {
        LOG(ERROR) << "Error while loading script file of " << id_ << ": result code " << result;
        status_ = j_script_status::error;
    }
}

j_script::j_script(const std::string& id, const std::string& content)
    : id_(id) {
    state_ = luaL_newstate();

    if (!state_) {
        LOG(ERROR) << "Could not allocate new lua state";
        status_ = j_script_status::error;
        return;
    }
    const auto result { luaL_loadstring(state_, content.c_str()) };

    if (result == LUA_OK) {
        status_ = j_script_status::loaded;
    } else {
        LOG(ERROR) << "Error while loading script " << id_ << ": result code " << result;
        status_ = j_script_status::error;
    }
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
    // TODO: improve
    switch (lua_pcall(state_, 0, 0, 0)) {
    case LUA_ERRRUN:
        LOG(ERROR) << "Runtime error in script " << id_ << ": " << lua_tostring(state_, lua_gettop(state_));
        return false;
    }
    return true;
}

j_script_status j_script::status() const noexcept {
    return status_;
}

bool j_script::loaded() const noexcept {
    return status_ == j_script_status::loaded;
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
    return *this;
}
