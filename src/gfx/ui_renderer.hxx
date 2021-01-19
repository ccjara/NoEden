#ifndef JARALYN_UI_RENDERER_HXX
#define JARALYN_UI_RENDERER_HXX

#include "./display.hxx"
#include "../hud/hud_system.hxx"

class j_ui_renderer {
private:
    j_display* display_ { nullptr };
    j_hud_system* hud_;

    void draw_node(j_ui_node* node);
public:
    explicit constexpr j_ui_renderer(
        j_display* display,
        j_hud_system* hud
    ) : display_(display), hud_(hud) {
        assert(display_);
        assert(hud_);
    }

    void draw();
};

#endif
