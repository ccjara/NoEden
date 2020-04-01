#ifndef JARALYN_OBJECT_HXX
#define JARALYN_OBJECT_HXX

struct object_deprecated {
    bool is_solid { false };
    bool is_void { true };
    char debug_char { 0 };
};

constexpr object_deprecated null_object;

#endif