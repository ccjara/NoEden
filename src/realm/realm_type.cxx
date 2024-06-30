#include "realm/realm_type_def.hxx"

const std::unordered_map<std::string, RealmType> realm_type_map = {
    {"MAIN_MENU", RealmType::MainMenu},
    {"WORLD", RealmType::World},
};

const std::unordered_map<RealmType, std::string> realm_key_map = {
    {RealmType::MainMenu, "MAIN_MENU"},
    {RealmType::World, "WORLD"},
};

RealmType realm_type_from_string(const std::string& str) {
    const auto it = realm_type_map.find(str);
    if (it == realm_type_map.end()) {
        return RealmType::MainMenu;
    }
    return it->second;
}

const std::string& realm_type_to_string(RealmType type) {
    const auto it = realm_key_map.find(type);
    if (it == realm_key_map.end()) {
        return realm_key_map.at(RealmType::MainMenu);
    }
    return it->second;
}
