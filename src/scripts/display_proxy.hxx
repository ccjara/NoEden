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

    /**
     * text(string text, uint x, uint y, [int color])
     */
    void text(const char* text, uint32_t x, uint32_t y, lua_State* state) {
        constexpr const uint32_t i = 5; // start of optional params
        j_color color;

        if (lua_type(state, i) == LUA_TNUMBER) {
            color = j_color(lua_tonumber(state, i));
        }
        display_->text(text, j_rect<uint32_t>(y, 0, 0, x), color);
    }
};

#endif
