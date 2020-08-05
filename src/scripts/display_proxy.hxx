#ifndef JARALYN_DISPLAY_PROXY_HXX
#define JARALYN_DISPLAY_PROXY_HXX

#include "../gfx/display.hxx"

class j_display_proxy {
private:
    j_display* display_;
public:
    constexpr j_display_proxy(j_display* display) : display_(display) {
        assert(display_);
    }

    void text(const char* text, uint32_t top, uint32_t right, uint32_t bottom, uint32_t left) {
        display_->text(text, j_rect<uint32_t>(top, right, bottom, left), j_color(255, 0, 0));
    }
};

#endif
