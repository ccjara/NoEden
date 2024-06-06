#ifndef NOEDEN_UI_API_HXX
#define NOEDEN_UI_API_HXX

#include "../lua_api.hxx"
#include "../../../ui/ui.hxx"

class UiApi final : public LuaApi {
public:
    void on_register(Script* script) final override;
private:
    UiWindow* create_window(const char* id);
    UiText* create_text(const char* id);

    template<typename Node>
    Node* create_node(const char* id) {
        if (!id) {
            return nullptr;
        }
        Node* n = Ui::tree().create_node<Node>(nullptr, id);
        if (!n) {
            return nullptr;
        }
        return n;
    }

    void destroy_node(const char* id);
};

#endif
