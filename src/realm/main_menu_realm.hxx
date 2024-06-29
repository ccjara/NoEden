#ifndef NOEDEN_MAIN_MENU_REALM_HXX
#define NOEDEN_MAIN_MENU_REALM_HXX

#include "realm/realm.hxx"

class Ui;
class UiText;
class MainMenu;

class MainMenuRealm : public Realm {
public:
    MainMenuRealm();

    ~MainMenuRealm() override;

    bool initialize() override;
    void update() override;
    void unload() override;
    void load() override;
private:
    std::unique_ptr<MainMenu> menu_;
};

#endif
