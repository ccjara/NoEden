#include "dictionary.hxx"

Dictionary::Dictionary(Container&& dict) : dict_(std::move(dict)) {
}

std::optional<std::reference_wrapper<const std::string>> Dictionary::get(const std::string& key) const {
    auto it = dict_.find(key);
    if (it == dict_.end()) {
        return std::nullopt;
    }
    return it->second;
}
