#ifndef JARALYN_NULL_SCENE_HXX
#define JARALYN_NULL_SCENE_HXX

#include "base_scene.hxx"

class j_null_scene : public j_base_scene {
public:
	void render(j_display& display) override {
	}

	void update(j_input_state& input) override {
	}
};

#endif
