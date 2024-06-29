#include "realm/realm_manager.hxx"

RealmManager::RealmManager(ServiceLocator* services, EventManager* events) :
    services_(services),
    events_(events) {
    assert(services_);
    assert(events_);
}


bool RealmManager::add_realm(std::unique_ptr<Realm>&& realm) {
    auto type = realm->type();
    auto result = realms.try_emplace(type, std::move(realm));
    if (!result.second) {
        LOG_ERROR("Realm {} already exists", static_cast<i32>(type));
        return false;
    }

    auto& inserted_realm = result.first->second;

    inserted_realm->services_ = services_;
    inserted_realm->events_ = events_;

    return inserted_realm->initialize();
}

bool RealmManager::switch_realm(RealmType type) {
    auto it = realms.find(type);
    if (it == realms.end()) {
        LOG_ERROR("Realm {} does not exist", static_cast<i32>(type));
        return false;
    }

    if (current_realm_) {
        current_realm_->unload();
        current_realm_ = nullptr;
    }

    current_realm_ = it->second.get();
    current_realm_->load();
    return true;
}
