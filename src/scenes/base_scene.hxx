#ifndef JARALYN_SCENE_HXX
#define JARALYN_SCENE_HXX

#include "scene_interface.hxx"

/**
 * @brief Extension point for any game scene
 */
class j_base_scene : public j_scene {
protected:
	const j_scene_type type_;
	entt::dispatcher* game_events_ { nullptr };
	j_scene_writer* scene_writer_ { nullptr };

	entt::registry registry_;

	bool is_opaque_ { false };
	bool is_blocking_ { false };


	j_base_scene(j_scene_type type) : type_(type) {
	}
public:
	virtual ~j_base_scene() = default;

	void attach(j_scene_writer* const, entt::dispatcher* const game_events) override;

	[[nodiscard]] j_scene_type type() const noexcept;

	void on_create() override;

	bool opaque() const noexcept override;

	bool blocking() const noexcept override;
};

#endif
