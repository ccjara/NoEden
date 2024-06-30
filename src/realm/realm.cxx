#include "realm/realm.hxx"

Realm::Realm(RealmType type) : type_(type) {
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
