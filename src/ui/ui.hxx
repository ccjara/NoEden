#ifndef JARALYN_UI_HXX
#define JARALYN_UI_HXX

#include "../gfx/gfx_event.hxx"
#include "../scripts/script_event.hxx"
#include "../gfx/display.hxx"
#include "ui_tree.hxx"

class Ui {
public:
    explicit Ui(EventManager& dispatcher, Display& display);

    void startup();
    void shutdown();
    void update();

    void draw_node(UiNode* node);

    UiTree& ui_tree();
private:
    EventManager& events_;
    Display& display_;
    UiTree ui_tree_;

    bool on_display_resized(DisplayResizedEvent& e);
    bool on_script_reset(ScriptResetEvent& e);
    bool on_post_world_render(PostWorldRenderEvent& e);
};

#endif
