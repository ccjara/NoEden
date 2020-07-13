#include "script.hxx"

j_script::j_script() {
    state_ = luaL_newstate();
}

j_script::~j_script() {
    if (state_) {
        lua_close(state_);
    }
}

j_script::operator lua_State* () const noexcept {
    return state_;
}

j_script j_script::from_file(const char* path) {
    j_script script;

    if (!script.state_) {
        LOG(ERROR) << "Could not allocate new lua state";
        script.status_ = j_script_status::error;
        return script;
    }
    const auto result { luaL_loadfilex(script.state_, path, nullptr) };

    if (result == LUA_OK) {
        script.status_ = j_script_status::loaded;
    } else {
        LOG(ERROR) << "Error while loading script file: result code " << result;
        script.status_ = j_script_status::error;
    }

    return script;
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
    if (other.state_ != nullptr) {
        state_ = std::exchange(other.state_, nullptr);
    }
    status_ = other.status_;
    return *this;
}
