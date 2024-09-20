#ifndef NOEDEN_PLAIN_RESOURCE_REPOSITORY_HXX
#define NOEDEN_PLAIN_RESOURCE_REPOSITORY_HXX

#include "resource/resource_repository.hxx"

/**
 * @brief A resource repository that loads files from disk directly
 */
class PlainResourceRepository : public ResourceRepository {
public:
    /**
     * @brief Loads a file from the specified file system path
     */
    [[nodiscard]] std::optional<std::vector<u8>> load_from_path(std::string_view path) override;
};

#endif
