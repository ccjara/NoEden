#include "realm/realm_manager.hxx"
#include "realm/realm_event.hxx"

RealmManager::RealmManager(ServiceLocator* services, EventManager* events) :
    services_(services),
    events_(events) {
    assert(services_);
    assert(events_);
}


bool RealmManager::add_realm(std::unique_ptr<Realm>&& realm) {
    const RealmType type = realm->type();
    if (realms_.contains(type)) {
        LOG_ERROR("Realm {} already exists", realm_type_to_string(type));
        return false;
    }

    realm->services_ = services_;
    realm->events_ = events_;
    if (!realm->initialize()) {
        return false;
    }
    realms_[type] = std::move(realm);
    return true;
}

bool RealmManager::switch_realm(RealmType type) {
    auto it = realms_.find(type);
    if (it == realms_.end()) {
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

const std::unordered_map<RealmType, std::unique_ptr<Realm>>& RealmManager::realms() const {
    return realms_;
}

const Realm* RealmManager::current_realm() const {
    return current_realm_;
}

Realm* RealmManager::current_realm() {
    return current_realm_;
}
