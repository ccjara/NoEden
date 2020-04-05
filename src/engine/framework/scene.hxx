#ifndef JARALYN_SCENE_HXX
#define JARALYN_SCENE_HXX

#include "object.hxx"

class scene {
protected:
	scene_id_t scene_id { 0 };
	std::vector<object> objects_;
public:
	virtual ~scene() = default;
};

#endif
