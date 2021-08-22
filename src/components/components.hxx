#ifndef JARALYN_COMPONENTS_HXX
#define JARALYN_COMPONENTS_HXX

#include "attribute.hxx"
#include "attribute_bearing.hxx"
#include "item.hxx"
#include "../ai/ai_node.hxx"

struct jc_object_descriptor {
    std::string label;
};

using jc_position = j_vec2<uint32_t>;

struct jc_renderable {
    uint32_t glyph;
    j_color color;
    bool visible { true };
};

struct jc_ai {
    std::unique_ptr<j_ai_node> root;

    explicit jc_ai(std::unique_ptr<j_ai_node>&& node) : root { std::move(node) } {
        assert(root);
    }
};

#endif
