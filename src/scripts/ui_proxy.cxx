#include "ui_proxy.hxx"

j_ui_proxy::j_ui_proxy(j_ui_tree* tree) : tree_(tree) {
    assert(tree_);
}

j_ui_window_proxy j_ui_proxy::create_window(const char* id) {
    auto window = tree_->create_window(nullptr, id);

    return j_ui_window_proxy { window };
}