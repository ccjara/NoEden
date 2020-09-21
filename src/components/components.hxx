#ifndef JARALYN_COMPONENTS_HXX
#define JARALYN_COMPONENTS_HXX

#include "attribute.hxx"
#include "attribute_bearing.hxx"
#include "inventory.hxx"

struct jc_object_descriptor {
    std::string label;
};

struct jc_position {
    int32_t x;
    int32_t y;
    int32_t z;
};

struct jc_renderable {
    std::string text;
    j_text_options text_options;
    bool visible { true };

    // TODO refactor: move to dedicated factory
    inline static jc_renderable unit(unsigned char glyph, j_color color) {
        jc_renderable r;
        r.text = glyph;
        r.text_options.color = color;
        return r;
    }

    // TODO refactor: move to dedicated factory
    inline static jc_renderable item(unsigned char glyph, j_color color) {
        jc_renderable r;
        r.text = glyph;
        r.text_options.color = color;
        return r;
    }
};

#endif
