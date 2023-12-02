#ifndef JARALYN_TRANSLATOR_HXX
#define JARALYN_TRANSLATOR_HXX

class Translator {
public:
    using translation_dict = std::unordered_map<std::string, std::string>;

    template<typename key, typename... args_type>
    static std::string translate(const key& s, const args_type&&... args) {
        auto it = db_.find(s);
        if (it == db_.end()) {
            return std::string(s);
        }
        return fmt::format(fmt::runtime(it->second), std::forward<args_type>(args)...);
    }

    static void load(std::string_view lang_file);
private:
    static translation_dict db_;
};

#endif
