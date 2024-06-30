#include "main_menu/main_menu_realm.hxx"
#include "realm/realm_type_def.hxx"
#include "main_menu/main_menu.hxx"

MainMenuRealm::MainMenuRealm() : Realm(RealmType::MainMenu) {
}

MainMenuRealm::~MainMenuRealm() {
}

bool MainMenuRealm::initialize() {
    return true;
}

void MainMenuRealm::load() {
    menu_ = std::make_unique<MainMenu>();
}

void MainMenuRealm::unload() {
    menu_.reset();
}

void MainMenuRealm::update() {
}
