#ifndef JARALYN_ATTRIBUTE_BEARING_HXX
#define JARALYN_ATTRIBUTE_BEARING_HXX

#include "attribute.hxx"

class jc_attribute_bearing {
protected:
    std::unordered_map<j_attribute_type, j_attribute> attributes_;
public:
    jc_attribute_bearing();

    void modify(j_attribute_type type, int32_t value);

    const j_attribute& attribute(j_attribute_type type);

    const std::unordered_map<j_attribute_type, j_attribute>& attributes() const;
};

#endif
