#include "realm/realm.hxx"

Realm::Realm(RealmType type) : type_(type), realm_services_(std::make_unique<ServiceLocator>()) {
    assert(realm_services_);
}

RealmType Realm::type() const {
    return type_;
}

bool Realm::initialize() {
    return true;
}

void Realm::update() {
}

void Realm::unload() {
}

void Realm::load() {
}

ServiceLocator& Realm::services() {
    return *realm_services_;
}

const ServiceLocator& Realm::services() const {
    return *realm_services_;
}
