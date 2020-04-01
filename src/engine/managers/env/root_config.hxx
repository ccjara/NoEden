#ifndef ROOT_CONFIG_HXX
#define ROOT_CONFIG_HXX

class root_config {
private:
    size<uint32_t> resolution_;

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

    size<uint32_t> resolution() const noexcept {
        return resolution_;
    }
};

#endif