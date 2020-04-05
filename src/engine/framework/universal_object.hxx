#ifndef JARALYN_UNIVERSAL_OBJECT
#define JARALYN_UNIVERSAL_OBJECT

#include "object.hxx"

class universal_object : public object {
private:
    std::vector<const object*> extensions_;
public:
    void extend(const object* o);
};

#endif
