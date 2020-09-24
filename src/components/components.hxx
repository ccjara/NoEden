#ifndef JARALYN_COMPONENTS_HXX
#define JARALYN_COMPONENTS_HXX

#include "attribute.hxx"
#include "attribute_bearing.hxx"
#include "item.hxx"

struct jc_object_descriptor {
    std::string label;
};

using jc_position = j_vec2<uint32_t>;

struct jc_renderable {
    uint32_t glyph;
    j_color color;
    bool visible { true };
};

#endif
