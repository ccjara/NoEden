#pragma once

#include "resource/resource_repository.hxx"

/**
 * @brief A resource repository that loads files from disk directly
 */
class PlainResourceRepository : public ResourceRepository {
public:
    [[nodiscard]] std::optional<std::vector<u8>> load_from_path(std::string_view path) override;
    [[nodiscard]] std::vector<std::string> list_files(std::string_view path) const override;
    [[nodiscard]] std::vector<std::string> list_files(std::string_view path, std::string_view ending) const override;
};
