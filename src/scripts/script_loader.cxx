#include "scripts/script_loader.hxx"
#include "scripts/script.hxx"

LoadFromDirectoryResult ScriptLoader::load_from_directory(std::string_view path) const {
    const auto abs_path = fs::absolute(path);

    if (!fs::is_directory(abs_path)) {
        return {{}, LoadFromDirectoryError::PathNotDirectory };
    }
    LoadFromDirectoryResult result;

    for (const auto& entry: fs::recursive_directory_iterator(abs_path, fs::directory_options::skip_permission_denied)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        const auto& file_path = entry.path();

        if (file_path.extension().string() != ".lua") {
            continue;
        }
        std::ifstream input(file_path, std::ios::ate);
        if (!input) {
            continue;
        }
        std::string name = script_name(abs_path, entry);
        std::string source;

        const auto size = input.tellg();
        source.resize(size);
        input.seekg(0);
        input.read(source.data(), size);

        result.scripts.emplace_back(std::make_unique<Script>(std::move(name), std::move(source)));
    }

    return result;
}

std::string ScriptLoader::script_name(
    const fs::path& base_path,
    const fs::directory_entry& entry
) const {
    fs::path relative_path = fs::relative(entry.path(), base_path);
    fs::path parent_path = relative_path.parent_path();

    if (parent_path.empty()) {
        return relative_path.stem().string();
    }
    return parent_path.string() + "/" + relative_path.stem().string();
}
