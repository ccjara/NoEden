#ifndef JARALYN_ROOT_CONFIG_HXX
#define JARALYN_ROOT_CONFIG_HXX

#include "../scripts/script.hxx"

/**
 * @brief Contains platform / system options for the game
 */
class j_root_config {
private:
    j_size<uint32_t> resolution_;
public:
    /**
     * @brief Initializes the configuration defaults
     */
    j_root_config();

    /**
     * @brief Loads the configuration from a loaded script
     */
    explicit j_root_config(j_script& sys_script);

    [[nodiscard]] j_size<uint32_t> resolution() const noexcept {
        return resolution_;
    }
};

#endif
