#include "resource_manager.hxx"

void resource_manager::startup() {
}

void resource_manager::shutdown() noexcept {
}

std::ifstream resource_manager::load_file(const std::string& path) const {
    std::ifstream f(path);

    return f;
}