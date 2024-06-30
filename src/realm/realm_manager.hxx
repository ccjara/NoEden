#ifndef NOEDEN_REALM_MANAGER_HXX
#define NOEDEN_REALM_MANAGER_HXX

#include "realm/realm.hxx"

class RealmManager {
public:
    RealmManager(ServiceLocator* services, EventManager* events);

    [[nodiscard]] bool add_realm(std::unique_ptr<Realm>&& realm);

    [[nodiscard]] bool switch_realm(RealmType type);

    [[nodiscard]] const std::unordered_map<RealmType, std::unique_ptr<Realm>>& realms() const;
    [[nodiscard]] const Realm* current_realm() const;
    [[nodiscard]] Realm* current_realm();
private:
    std::unordered_map<RealmType, std::unique_ptr<Realm>> realms_;

    Realm* current_realm_ = nullptr;
    ServiceLocator* services_ = nullptr;
    EventManager* events_ = nullptr;
};

#endif
