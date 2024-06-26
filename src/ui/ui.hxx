#ifndef NOEDEN_UI_HXX
#define NOEDEN_UI_HXX

#include "../gfx/gfx_event.hxx"
#include "../scripts/script_event.hxx"
#include "ui_tree.hxx"
#include "ui_window.hxx"
#include "ui_text.hxx"

class Display;

class Ui {
public:
    static void init(EventManager* events, Display* display);
    static void shutdown();
    static void update();
    static void draw();

    static void draw_node(UiNode* node);

    static UiTree& tree();
private:
    Ui() = delete;

    static inline UiTree ui_tree_;
    static inline EventManager* events_ = nullptr;
    static inline Display* display_ = nullptr;

    static void update_node(UiNode* node);

    static EventResult on_display_resized(DisplayResizedEvent& e);
    static EventResult on_script_reset(ScriptResetEvent& e);

    static inline Subscription<DisplayResizedEvent> display_resized_sub_;
    static inline Subscription<ScriptResetEvent> script_reset_sub_;
};

#endif
