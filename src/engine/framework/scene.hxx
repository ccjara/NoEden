#ifndef JARALYN_SCENE_HXX
#define JARALYN_SCENE_HXX

#include "object.hxx"
#include "game_system.hxx"

using scene_id_t = int32_t;

class scene {
protected:
	scene_id_t scene_id { 0 };
	std::vector<object> objects_;
public:
	virtual ~scene() = default;

	virtual const std::list<system_id_t> list_required_systems() const = 0;
};

#endif
