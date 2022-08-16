#ifndef JARALYN_UI_API_HXX
#define JARALYN_UI_API_HXX

#include "../lua_api.hxx"
#include "../../../ui/ui_tree.hxx"
#include "../../../ui/ui_window.hxx"
#include "../../../ui/ui_text.hxx"

class UiApi final : public LuaApi {
public:
    explicit UiApi(UiTree& ui_tree);

    void on_register(Script* script) final override;
private:
    UiWindow* create_window(const char* id);
    UiText* create_text(const char* id);

    template<typename Node>
    Node* create_node(const char* id) {
        if (!id) {
            return nullptr;
        }
        auto n { ui_tree_.create_node<Node>(nullptr, id) };
        if (!n) {
            return nullptr;
        }
        return n;
    }

    void destroy_node(const char* id);

    UiTree& ui_tree_;
};

#endif
