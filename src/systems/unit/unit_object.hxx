#ifndef JARALYN_UNIT_OBJECT_HXX
#define JARALYN_UNIT_OBJECT_HXX

#include "../object_definition.hxx"

class unit_object : public object {
public:
    int value { 8 };

    unit_object() {
        type_ = object_type::o_unit;
    }
};

#endif
