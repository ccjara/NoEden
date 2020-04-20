#ifndef JARALYN_COMPONENTS_HXX
#define JARALYN_COMPONENTS_HXX

#include "attribute.hxx"

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


class jc_attribute_bearing {
protected:
    std::unordered_map<j_attribute_type, j_attribute> attributes_;

    j_attribute& resolve(j_attribute_type type) {
        attributes_.try_emplace(type, j_attribute({ type, 0 }));

        return attributes_[type];
    }
public:
    void modify(j_attribute_type type, int32_t value) {
        auto& attr { resolve(type) };

        attr.points += value;
    }

    const j_attribute& attribute(j_attribute_type type) noexcept {
        return resolve(type);
    }

    const std::unordered_map<j_attribute_type, j_attribute>& attributes() const noexcept {
        return attributes_;
    }
};

#endif
