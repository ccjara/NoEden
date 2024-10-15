#include "resource/plain_resource_repository.hxx"

std::optional<std::vector<u8>> PlainResourceRepository::load_from_path(std::string_view path) {
    if (!fs::exists(path)) {
        LOG_ERROR("Could not load file from path {}: not found", path);
        return std::nullopt;
    }

    if (!fs::is_regular_file(path)) {
        LOG_ERROR("Could not load file from path {}: not a regular file", path);
        return std::nullopt;
    }

    auto file = std::ifstream(path.data(), std::ios::binary | std::ios::ate);
    if (!file) {
        LOG_ERROR("Could not load file from path {}: could not open file for reading", path);
        return std::nullopt;
    }

    const auto size = file.tellg();
    if (size == 0) {
        return std::vector<u8>();
    }
    std::vector<u8> buffer(size);
    file.seekg(0);
    file.read(reinterpret_cast<char*>(buffer.data()), size);
    return buffer;
}

std::vector<std::string> PlainResourceRepository::list_files(std::string_view path, std::string_view ending) const {
    std::vector<std::string> files;
    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (entry.is_regular_file() && entry.path().extension() == ending) {
            files.push_back(entry.path().string());
        }
    }
    return files;
}

std::vector<std::string> PlainResourceRepository::list_files(std::string_view path) const {
    std::vector<std::string> files;
    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().string());
        }
    }
    return files;
}
