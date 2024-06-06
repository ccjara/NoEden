#ifndef JARALYN_SCRIPT_LOADER_HXX
#define JARALYN_SCRIPT_LOADER_HXX

class Script;

enum class LoadFromDirectoryError;
struct LoadFromDirectoryResult;

class ScriptLoader {
public:
    /**
     * @brief Recursively loads all scripts from the given directory path.
     *
     * Script files must have a lowercased `.lua` extension.
     *
     * The script name will be constructed from the file path stem (filename
     * without extension) and its directory level.
     *
     * Examples (assuming `scripts` as the `base_path`):
     *   - `scripts/extra/optional.lua` -> `extra/optional`
     *   - `scripts/very/deep/file.lua` -> `very/deep/file`
     *   - `scripts/system.lua` -> `system`
     *
     *  @param path The directory path to load scripts from
     *
     *  @return A result containing the loaded scripts and an error code
     */
    [[nodiscard]] LoadFromDirectoryResult load_from_directory(std::string_view path) const;
private:
    [[nodiscard]] inline std::string script_name(
        const fs::path& base_path,
        const fs::directory_entry& entry
    ) const;
};

enum class LoadFromDirectoryError {
    None,
    PathNotDirectory,
};

struct LoadFromDirectoryResult {
    std::vector<std::unique_ptr<Script>> scripts;
    LoadFromDirectoryError error = LoadFromDirectoryError::None;
};

#endif
