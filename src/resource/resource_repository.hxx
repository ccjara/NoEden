#ifndef NOEDEN_RESOURCE_REPOSITORY_HXX
#define NOEDEN_RESOURCE_REPOSITORY_HXX

class ResourceRepository {
public:
    /**
     * @brief Loads a file from the specified path
     */
    [[nodiscard]] virtual std::optional<std::vector<u8>> load_from_path(std::string_view path) = 0;

    virtual ~ResourceRepository() = default;
};

#endif
