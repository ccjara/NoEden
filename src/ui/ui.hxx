#ifndef JARALYN_UI_HXX
#define JARALYN_UI_HXX

#include "../gfx/gfx_event.hxx"
#include "../scripts/script_event.hxx"
#include "ui_tree.hxx"

class Ui {
public:
    explicit Ui(entt::dispatcher& dispatcher);

    void startup();
    void shutdown();
    void update();

    UiTree& ui_tree();
private:
    entt::dispatcher& dispatcher_;
    UiTree ui_tree_;

    void on_display_resized(const DisplayResizedEvent& e);
    void on_script_reset(const ScriptResetEvent& e);
};

#endif
