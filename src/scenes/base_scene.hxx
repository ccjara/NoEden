#ifndef JARALYN_SCENE_HXX
#define JARALYN_SCENE_HXX

#include "../gfx/gfx_system.hxx"
#include "../input/input_interface.hxx"
#include "scene_interface.hxx"

/**
 * @brief Extension point for any game scene
 */
class j_base_scene : public j_scene {
protected:
	entt::dispatcher* game_events_ { nullptr };
	j_scene_writer* scene_writer_ { nullptr };

	entt::registry registry_;

	bool is_opaque_ { false };
	bool is_blocking_ { false };
public:
	virtual ~j_base_scene() = default;

	void attach(j_scene_writer* const, entt::dispatcher* const game_events) override;

	bool opaque() const noexcept override;

	bool blocking() const noexcept override;
};

#endif
