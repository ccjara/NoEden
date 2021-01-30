#ifndef JARALYN_WORLD_RENDERER_HXX
#define JARALYN_WORLD_RENDERER_HXX

#include "../game.hxx"
#include "../components/components.hxx"
#include "../gfx/display.hxx"

class j_world_renderer {
    j_display* display_ { nullptr };
public:
    explicit constexpr j_world_renderer(
        j_display* display
    ) : display_(display) {
        assert(display_);
    }
	
    void draw();
};

#endif
