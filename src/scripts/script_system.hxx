#ifndef JARALYN_SCRIPT_SYSTEM_HXX
#define JARALYN_SCRIPT_SYSTEM_HXX

#include "../system.hxx"
#include "../event/script_event.hxx"
#include "../event/platform_event.hxx"
#include "script.hxx"

class j_script_system : public j_system<j_script_system> {
private:
    std::unordered_map<j_id_t, std::unique_ptr<j_script>> scripts_;

    // internal testing
    void on_key_down(const j_key_down_event& e);

    /**
     * @brief Loads and runs the script
     *
     * TODO: Make scripts loadable on demand
     * TODO: handle reloaded internally by checking if a script was loaded already
     */
    void setup(j_script& script, bool reloaded);

    /**
     * @brief Resets the script system to its initial state
     *
     * In this state, no script or any reference thereof is held
     */
    void reset();

    /**
     * @brief Does a protected call into the given lua ref
     *
     * Checks whether the call was successful. In case of an error, the error
     * is logged, *no* exception is thrown.
     */
    template<typename... varg_t>
    inline void pcall_into(luabridge::LuaRef& ref, varg_t&&... args) const;
public:
    constexpr static const char* default_script_path {
#ifdef NDEBUG
        "scripts"
#else
        "../src/scripts/lua"
#endif
    };

    /**
     * @brief Unloads all scripts before freeing resources
     */
    ~j_script_system();

    /**
     * @brief Recursively preloads all scripts from the given directory path
     *
     * Script files must have a lowercased `.lua` extension.
     *
     * The script name will be constructed from the file path stem (filename
     * without extension) and its directory level.
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

    /**
     * @brief Unloads a script by id
     *
     * An unloaded script can later be reloaded.
     *
     * @see reload
     */
    void unload(j_id_t id);

    /**
     * @brief Reloads a script by id
     *
     * The script must be known to the script engine.
     * Scripts which have been added after {@link preload} was called are not
     * known to the script system (TODO).
     *
     * If the script was loaded from a file, the source will also be reloaded
     * from the file system, making changes at runtime possible.
     */
    void reload(j_id_t id);

    void on_load() override;
    void update(uint32_t delta_time) override;

    const std::unordered_map<j_id_t, std::unique_ptr<j_script>>& scripts() const;
};

template<typename path_like>
void j_script_system::preload(path_like base_path) {
    reset();

    const auto abs_path { fs::absolute(base_path) };

    if (!fs::is_directory(abs_path)) {
        LOG(ERROR) << "Script path " << abs_path << " must be a readable directory";
        std::abort();
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
            const std::string& name { prefix + path.stem().string() };

            LOG(INFO) << "Found script " << name;

            auto script { std::make_unique<j_script>(name, path) };
            // TODO: gracefully handle case insensitive file systems (script ids must be unique)
            // TODO: locale-lowercase script_id for convenience and consistency?
            // TODO: check against unicode file names
            auto [iter, b] { scripts_.try_emplace(script->id(), std::move(script)) };
            setup(*iter->second, false);
        }
    }
}

template<typename... varg_t>
inline void j_script_system::pcall_into(luabridge::LuaRef& ref, varg_t&&... args) const {
    const auto result { ref(std::forward<varg_t>(args)...) };
    if (result == std::nullopt) {
        const auto state { ref.state() };

        std::string err { "Caught error during script execution" };

        if (lua_gettop(state) > 0) {
            const char* e = lua_tostring(state, -1);
            if (e) {
                err.append(": ");
                err.append(e);
            }
        }
        LOG(ERROR) << err;
    }
}

#endif
