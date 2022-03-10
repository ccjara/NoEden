#ifndef JARALYN_UI_HXX
#define JARALYN_UI_HXX

#include "../gfx/gfx_event.hxx"
#include "../scripts/script_event.hxx"
#include "ui_tree.hxx"

class Ui {
public:
    explicit Ui(EventManager& dispatcher);

    void startup();
    void shutdown();
    void update();

    UiTree& ui_tree();
private:
    EventManager& events_;
    UiTree ui_tree_;

    bool on_display_resized(DisplayResizedEvent& e);
    bool on_script_reset(ScriptResetEvent& e);
};

#endif
