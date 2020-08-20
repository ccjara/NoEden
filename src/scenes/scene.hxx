#ifndef JARALYN_SCENE_HXX
#define JARALYN_SCENE_HXX

#include "scene_writer.hxx"
#include "../gfx/display.hxx"
#include "../input/input_state.hxx"
#include "../identity.hxx"

/**
 * @brief Extension point for any game scene
 */
class j_scene : public j_identity<j_scene> {
protected:
	const j_scene_type type_;
	entt::dispatcher* game_events_ { nullptr };
	j_scene_writer* scene_writer_ { nullptr };

	entt::registry registry_;

	bool is_opaque_ { false };
	bool is_blocking_ { false };


	j_scene(j_scene_type type) : type_(type) {
	}
public:
	virtual ~j_scene() = default;

	j_scene_type type() const noexcept;

	/**
	 * @brief Called as soon as the scene composer creates the scene
	 *
	 * This method is called before {@see j_scene_created_event} is triggered,
	 * allowing the scene to properly initialize itself before broadcasting
	 * its creation.
	 *
	 * All dependencies of the scene are expected to be initialized at this point.
	 */
	virtual void on_create();

	/**
	 * @brief Called during the render phase of the game loop
	 */
	virtual void render(j_display& display) = 0;

	/**
	 * @brief Called during the update phase of the game loop
	 */
	virtual void update(j_input_state& input) = 0;

	/**
	 * @brief Attaches the scene to the scene writer
	 *
	 * The scene writer interface enables the scene to close itself (for example
	 * based on player input) or load a new scene which will be pushed onto
	 * the scene stack.
	 *
	 * The scene writer pointer is null before this method is called.
	 */
	void attach(j_scene_writer* const, entt::dispatcher* const game_events);

	/**
	 * @brief Returns true if this scene prevents underlying scenes from rendering
	 */
	bool opaque() const noexcept;

	/**
	 * @brief Returns true if this scene prevents underlying scenes from updating
	 */
	bool blocking() const noexcept;
};

#endif
