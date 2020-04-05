#include "universal_object.hxx"

void universal_object::extend(const object* o) {
    assert(o);
    extensions_.push_back(o);
}
