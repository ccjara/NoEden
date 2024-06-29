#ifndef NOEDEN_REALM_MANAGER_HXX
#define NOEDEN_REALM_MANAGER_HXX

#include "realm/realm.hxx"

class RealmManager {
public:
    RealmManager(ServiceLocator* services, EventManager* events);

    [[nodiscard]] bool add_realm(std::unique_ptr<Realm>&& realm);

    [[nodiscard]] bool switch_realm(RealmType type);
private:
    std::unordered_map<RealmType, std::unique_ptr<Realm>> realms;

    Realm* current_realm_ = nullptr;
    ServiceLocator* services_ = nullptr;
    EventManager* events_ = nullptr;
};

#endif
