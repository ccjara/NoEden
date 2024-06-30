#include "realm/realm_manager.hxx"
#include "realm/realm_event.hxx"

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
        LOG_ERROR("Realm {} already exists", realm_type_to_string(type));
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
        LOG_ERROR("Realm {} does not exist", realm_type_to_string(type));
        return false;
    }

    if (current_realm_) {
        current_realm_->unload();
        current_realm_ = nullptr;
    }

    current_realm_ = it->second.get();
    current_realm_->load();

    events_->trigger<RealmLoadedEvent>(current_realm_);

    LOG_INFO("Switched to realm {}", realm_type_to_string(type));

    return true;
}
