#ifndef JARALYN_SCENE_HXX
#define JARALYN_SCENE_HXX

#include "object.hxx"
#include "game_system.hxx"

using scene_id_t = int32_t;

class scene {
protected:
	scene_id_t scene_id { 0 };
	std::vector<object> objects_;
	std::vector<system_id_t> required_systems_;
public:
	virtual ~scene() = default;
};

#endif
