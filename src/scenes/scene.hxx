#ifndef JARALYN_SCENE_HXX
#define JARALYN_SCENE_HXX

#include "../gfx/gfx_system.hxx"
#include "../input/input_interface.hxx"

class j_scene {
protected:
	j_scene_id id_ { 0 };
	entt::registry registry_;
public:
	virtual ~j_scene() = default;

	virtual void render(j_display& display) = 0;
	virtual void update(j_input_state& input) = 0;
};

class j_null_scene : public j_scene {
public:
	void render(j_display& display) override;
	void update(j_input_state& input) override;
};

#endif
