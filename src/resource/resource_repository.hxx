#pragma once

class ResourceRepository {
public:
    /**
     * @brief Loads a file from the specified path
     */
    [[nodiscard]] virtual std::optional<std::vector<u8>> load_from_path(std::string_view path) = 0;

    /**
     * @brief Lists all files in the given path
     */
    [[nodiscard]] virtual std::vector<std::string> list_files(std::string_view path) const = 0;

    /**
     * @brief Lists all files in the given path having the specified ending
     */
    [[nodiscard]] virtual std::vector<std::string> list_files(std::string_view path, std::string_view ending) const = 0;

    virtual ~ResourceRepository() = default;
};
