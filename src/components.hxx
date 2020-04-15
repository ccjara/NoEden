#ifndef JARALYN_COMPONENTS_HXX
#define JARALYN_COMPONENTS_HXX

struct jc_position {
    int32_t x;
    int32_t y;
    int32_t z;
};

struct jc_controllable {
};

struct jc_renderable {
    int32_t glyph { 0 };
    j_color color;
};

#endif
