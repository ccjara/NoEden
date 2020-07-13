#ifndef JARALYN_SCRIPT_SYSTEM_HXX
#define JARALYN_SCRIPT_SYSTEM_HXX

#include "script.hxx"

class j_script_system {
public:
    /**
     * @brief Loads a script from the given path
     *
     * Throws if the file does not exist
     */
    template<typename path_like>
    j_script require(path_like path);
};


template<typename path_like>
j_script j_script_system::require(path_like path) {
    const auto fs_path { std::filesystem::absolute(path) };

    if (!std::filesystem::exists(fs_path)) {
        LOG(ERROR) << "Could not find script file " << path;
        throw;
    }
    return j_script::from_file(fs_path.generic_u8string().c_str());
}

#endif
