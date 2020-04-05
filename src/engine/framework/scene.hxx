#ifndef JARALYN_SCENE_HXX
#define JARALYN_SCENE_HXX

#include "object.hxx"

using object_container = std::map<object_type_t, std::vector<std::unique_ptr<object>>>;

class scene {
protected:
	object_container objects_;

	scene_id_t scene_id { 0 };
public:
	virtual ~scene() = default;

	virtual void create_object(object&& obj) = 0;
	const object_container& objects() const noexcept;
};

#endif
