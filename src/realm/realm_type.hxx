#ifndef NOEDEN_REALM_TYPE_HXX
#define NOEDEN_REALM_TYPE_HXX

/**
* @brief Realm type, used to identify different realms. Game specific.
*/
enum class RealmType;

/**
* @brief Converts a string to a RealmType
*/
RealmType realm_type_from_string(const std::string& str);

/**
* @brief Converts a RealmType to a string
*/
const std::string& realm_type_to_string(RealmType type);

#endif
