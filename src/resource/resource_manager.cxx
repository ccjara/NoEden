#include "resource_manager.hxx"

std::ifstream j_resource_manager::load_file(const std::string& path) const {
    std::ifstream f(path);

    return f;
}

void j_resource_manager::startup() {
}

void j_resource_manager::shutdown() {
}
