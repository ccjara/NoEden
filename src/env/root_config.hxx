#ifndef JARALYN_ROOT_CONFIG_HXX
#define JARALYN_ROOT_CONFIG_HXX

#include "../scripts/script.hxx"

class j_root_config {
private:
    j_size<uint32_t> resolution_;
public:
    j_root_config();
    explicit j_root_config(j_script& sys_script);

    [[nodiscard]] j_size<uint32_t> resolution() const noexcept {
        return resolution_;
    }
};

#endif
