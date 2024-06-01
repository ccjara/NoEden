#ifndef JARALYN_TRANSLATION_LOADER_HXX
#define JARALYN_TRANSLATION_LOADER_HXX

#include "lang/dictionary.hxx"

struct LoadTranslationResult;

class TranslationLoader {
public:
    /**
     * @brief Loads the translation dictionary
     */
    [[nodiscard]] LoadTranslationResult load(std::string_view path) const;
};

enum class LoadTranslationError {
    None,
    FileCorrupted,
};

struct LoadTranslationResult {
    /**
     * @brief Constructs a successful result
     * @param dictionary Loaded dictionary
     */
    explicit LoadTranslationResult(Dictionary&& dictionary);

    /**
     * @brief Constructs a failed result with an empty dictionary
     * @param error Error code
     */
    explicit LoadTranslationResult(LoadTranslationError error);

    /**
     * @brief Whether the result is successful
     */
    LoadTranslationError error;

    /**
     * @brief Loaded dictionary or empty on error
     */
    Dictionary dictionary;
};

#endif
