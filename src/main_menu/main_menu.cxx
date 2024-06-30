#include "main_menu/main_menu.hxx"

MainMenu::MainMenu() {
    add_item(std::make_unique<MenuItem>(ItemType::CreateWorld, "MAIN_MENU_CREATE_WORLD", true));
    add_item(std::make_unique<MenuItem>(ItemType::Exit, "MAIN_MENU_EXIT", false));
}

void MainMenu::add_item(std::unique_ptr<MenuItem>&& item) {
    const auto type = item->type;

    items_.push_back(std::move(item));
    item_by_type_[type] = items_.back().get();
}

const MainMenu::MenuItem& MainMenu::item(ItemType type) const {
    return *item_by_type_.at(type);
}

const std::vector<std::unique_ptr<MainMenu::MenuItem>>& MainMenu::items() const {
    return items_;
}

const MainMenu::MenuItem& MainMenu::active_item() const {
    return *items_[active_item_index_];
}

void MainMenu::activate_next_item() {
    items_[active_item_index_]->active = false;
    active_item_index_ = (active_item_index_ + 1) % items_.size();
    items_[active_item_index_]->active = true;
}

void MainMenu::activate_previous_item() {
    items_[active_item_index_]->active = false;
    active_item_index_ = (active_item_index_ == 0) ? items_.size() - 1 : active_item_index_;
    items_[active_item_index_]->active = true;
}
