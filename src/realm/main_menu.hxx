#ifndef NOEDEN_MAIN_MENU_HXX
#define NOEDEN_MAIN_MENU_HXX

/**
* @brief Main menu (main menu realm)
*
* - Owned by MainMenuRealm
* - Controlled by MainMenuController
*/
class MainMenu {
public:
    enum class ItemType {
        CreateWorld,
        Exit,
    };

    struct MenuItem {
        ItemType type = ItemType::CreateWorld;

        std::string label = "";

        bool active = false;
    };

    MainMenu();

    [[nodiscard]] const MenuItem& active_item() const;

    [[nodiscard]] const std::vector<std::unique_ptr<MenuItem>>& items() const;

    [[nodiscard]] const MenuItem& item(ItemType type) const;

    void activate_next_item();

    void activate_previous_item();
private:
    void add_item(std::unique_ptr<MenuItem>&& item);

    std::vector<std::unique_ptr<MenuItem>> items_ = {};
    std::unordered_map<ItemType, MenuItem*> item_by_type_ = {};

    size_t active_item_index_ = 0;
};

#endif
