#ifndef NOEDEN_UI_API_HXX
#define NOEDEN_UI_API_HXX

#include "scripts/api/lua_api.hxx"

class Ui;
class UiWindow;
class UiText;

class UiApi : public LuaApi {
public:
    void on_register(Script& script) override;

    bool initialize() override;
private:
    Ui* ui_ = nullptr;

    UiWindow* create_window(const char* id);
    UiText* create_text(const char* id);

    void destroy_node(const char* id);
};

#endif
