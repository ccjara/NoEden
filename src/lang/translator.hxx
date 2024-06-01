#ifndef JARALYN_TRANSLATOR_HXX
#define JARALYN_TRANSLATOR_HXX

#include "dictionary.hxx"

class Translator {
public:
    explicit Translator(Dictionary&& dict);

    template<StringLike Key, typename... Args>
    std::string operator()(const Key& s, const Args&&... args) {
        return translate(s, std::forward<Args>(args)...);
    }

    template<StringLike Key, typename... Args>
    std::string translate(const Key& s, const Args&&... args) {
        std::string key(s);
        auto result = dict_.get(key);
        if (!result.has_value()) {
            return key;
        }
        return fmt::format(fmt::runtime(result.value().get()), std::forward<Args>(args)...);
    }
private:
    Dictionary dict_;
};

#endif
