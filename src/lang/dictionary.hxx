#ifndef NOEDEN_DICTIONARY_HXX
#define NOEDEN_DICTIONARY_HXX

class Dictionary {
public:
    using Container = std::unordered_map<std::string, std::string>;

    /**
     * Constructs a dictionary from the given container
     * @param dict loaded dictionary
     *
     * @see TranslationLoader
     */
    explicit Dictionary(Container&& dict);

    /**
     * Retrieves the value associated with the given key
     * @param key key to search for
     * @return translated string if found, empty otherwise
     */
    std::optional<std::reference_wrapper<const std::string>> get(const std::string& key) const;
private:
    Container dict_{};
};

#endif
