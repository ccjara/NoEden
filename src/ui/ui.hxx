#ifndef JARALYN_UI_HXX
#define JARALYN_UI_HXX

#include "../gfx/gfx_event.hxx"
#include "../scripts/script_event.hxx"
#include "../scripts/script.hxx"
#include "ui_proxy.hxx"

class Ui {
public:
    explicit Ui(entt::dispatcher& dispatcher);

    void update();

    UiTree& ui_tree();
private:
    entt::dispatcher& dispatcher_;
    UiTree ui_tree_;
    UiProxy ui_proxy_;

    void on_display_resized(const Display_resized_event& e);
    void on_script_loaded(const ScriptLoadedEvent& e);
    void on_script_reset(const ScriptResetEvent& e);
};

#endif
