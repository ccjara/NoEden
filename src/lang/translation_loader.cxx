#include "lang/translation_loader.hxx"

LoadTranslationResult::LoadTranslationResult(Dictionary&& dictionary) :
        dictionary(std::move(dictionary)),
        error(LoadTranslationError::None) {
}

LoadTranslationResult::LoadTranslationResult(LoadTranslationError error) :
        error(error),
        dictionary({}) {
}

LoadTranslationResult TranslationLoader::load(std::string_view path) const {
    auto parse_result = toml::parse_file(path);

    if (!parse_result) {
        return LoadTranslationResult(LoadTranslationError::FileCorrupted);
    }

    Dictionary::Container dict;
    for (const auto& [key, value]: parse_result.table()) {
        if (value.is_string()) {
            dict.emplace(key.str(), value.as_string()->get());
        }
    }

    return LoadTranslationResult(Dictionary(std::move(dict)));
}