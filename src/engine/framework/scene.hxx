#ifndef JARALYN_UNIVERSAL_SCENE
#define JARALYN_UNIVERSAL_SCENE

#include "object.hxx"

class scene {
protected:
	std::vector<scene> objects_;
public:
	virtual void initialize() = 0;
	virtual ~scene() = default;
};

#endif
