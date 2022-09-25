#ifndef JARALYN_CATALOG_HXX
#define JARALYN_CATALOG_HXX

#include "archetype.hxx"

class Catalog {
public:
    Catalog() = delete;

    /**
     * @brief Creates takes ownership of an empty archetype with the given name.
     *
     * You may modify the returned archetype further using the returned pointer.
     * Returns nullptr if an archetype with the given name already exists.
     */
    static Archetype* create_archetype(std::string_view name);

    /**
     * @brief Returns an Archetype by name or nullptr if the Archetype does not exist.
     */
    static const Archetype* archetype(std::string_view name);
protected:
    static inline std::unordered_map<std::string, std::unique_ptr<Archetype>> archetypes_;
};

#endif
