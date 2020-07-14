#ifndef JARALYN_SCRIPT_SYSTEM_HXX
#define JARALYN_SCRIPT_SYSTEM_HXX

#include "script.hxx"

class j_script_system {
private:
    std::unordered_map<std::string, j_script> cache_;
public:
    /**
     * @brief Recursively preloads all scripts from the given directory path
     *
     * Script files must have a lowercased `.lua` extension.
     *
     * The scripts will be stored as `j_script` instances inside `cache_` and
     * may be retrieved using a script id by calling the `require` method.
     * The id will be constructed from the file path stem (filename without
     * extension) and its directory level.
     * For each directory level visited, the stem will be prefixed with
     * the visited directory name.
     *
     * Examples (assuming `scripts` as the `base_path`):
     *   - `scripts/extra/optional.lua` -> `extra/optional`
     *   - `scripts/very/deep/file.lua` -> `very/deep/file`
     *   - `scripts/system.lua` -> `system`
     */
    template<typename path_like>
    void preload(path_like base_path);

    // TODO: reload

    /**
     * @brief Attempts to load the script with the given script id
     *
     * Throws if the file does not exist
     */
    template<typename string_like>
    j_script& require(string_like script_id);
};

template<typename path_like>
void j_script_system::preload(path_like base_path) {
    const auto abs_path { fs::absolute(base_path) };

    if (!fs::is_directory(abs_path)) {
        LOG(ERROR) << "Script path " << abs_path << " must be a readable directory";
        throw;
    }

    std::string prefix;

    // default options: does not follow symlinks, skips on denied permission
    for (const auto& entry : fs::recursive_directory_iterator(abs_path)) {
        const auto& path { entry.path() };
        const auto filename { path.filename().string() };

        if (entry.is_directory()) {
            prefix += filename + "/";
            continue;
        }
        if (path.extension().string() == ".lua") {
            const std::string& script_id = prefix + path.stem().string();

            LOG(INFO) << "Found script " << script_id;

            // TODO: gracefully handle case insensitive file systems (script ids must be unique)
            // TODO: locale-lowercase script_id for convenience and consistency?
            // TODO: check against unicode file names
            cache_.try_emplace(script_id, script_id, path);
        }
    }
}

template<typename string_like>
j_script& j_script_system::require(string_like script_id) {
    auto entry { cache_.find(script_id) };

    if (entry == cache_.end()) {
        LOG(ERROR) << "Could not find script " << script_id;
        throw;
    }

    return entry->second;
}

#endif
