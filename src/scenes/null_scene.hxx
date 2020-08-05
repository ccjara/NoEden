#ifndef JARALYN_NULL_SCENE_HXX
#define JARALYN_NULL_SCENE_HXX

#include "base_scene.hxx"

/**
 * @brief Fallback scene which contains no entities and will render nothing.
 */
class j_null_scene : public j_base_scene {
protected:
public:
	j_null_scene() : j_base_scene(j_scene_type::null) {
	}

	void render(j_display& display) override {
	}

	void update(j_input_state& input) override {
	}
};

#endif
