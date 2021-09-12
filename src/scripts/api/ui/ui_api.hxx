#ifndef JARALYN_UI_API_HXX
#define JARALYN_UI_API_HXX

#include "../lua_api.hxx"
#include "../../../ui/ui_tree.hxx"

class UiApi final : public LuaApi {
public:
    explicit UiApi(UiTree& ui_tree);

    void on_register(Script* script) final override;
private:
    UiWindow* create_window(const char* id);

    UiTree& ui_tree_;
};

#endif
