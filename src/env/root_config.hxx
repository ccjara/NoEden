#ifndef JARALYN_ROOT_CONFIG_HXX
#define JARALYN_ROOT_CONFIG_HXX

class j_root_config {
private:
    j_size<uint32_t> resolution_;

    inline void load_fallback() noexcept {
        resolution_.width = 800;
        resolution_.height = 600;
    }
public:
    void read(std::istream& input) {
        const auto fail = [this](std::string_view reason) noexcept {
            LOG(ERROR) << "Cannot read config: " << reason << ". Loading fallback configuration.";
            load_fallback();
        };

        if (!input) {
            return fail("Bad input stream");
        }
        fail("Reading configuration is not yet implemented");
    }

    j_size<uint32_t> resolution() const noexcept {
        return resolution_;
    }
};

#endif