#ifndef NOEDEN_UI_HXX
#define NOEDEN_UI_HXX

#include "gfx/gfx_event.hxx"
#include "scripts/script_event.hxx"
#include "ui/ui_tree.hxx"

class UiNode;
class UiTree;
class Renderer;
class Display;

class Ui {
public:
    Ui(Renderer* renderer, EventManager* events);

    bool initialize();

    void update();
    void draw();

    void draw_node(UiNode* node);

    UiTree& tree();
private:
    UiTree ui_tree_;
    EventManager* events_ = nullptr;
    Renderer* renderer_ = nullptr;
    Display* display_ = nullptr;

    void update_node(UiNode* node);

    EventResult on_display_resized(const DisplayResizedEvent& e);
    EventResult on_script_reset(const ScriptResetEvent& e);

    Subscription<DisplayResizedEvent> display_resized_sub_;
    Subscription<ScriptResetEvent> script_reset_sub_;
};

#endif
