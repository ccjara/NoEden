#ifndef JARALYN_SCENE_INTERFACE_HXX
#define JARALYN_SCENE_INTERFACE_HXX

#include "../gfx/display.hxx"
#include "../input/input_state.hxx"

class j_scene;
/**
 * @brief Partial (writing) interface of the scene stack manager
 */
class j_scene_writer {
public:
	/**
	 * @brief Loads a new scene and pushes it onto the scene stack
	 */
	virtual j_scene* load(j_scene_type type) = 0;

	/**
	 * @brief Unloads a scene by the given id
	 */
	virtual void unload(uint32_t id) = 0;
};

/**
 * @brief Stackable entity containers for the world, a menu or overlays
 */
class j_scene : public j_identity<j_scene> {
public:
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
	virtual void attach(j_scene_writer* const, entt::dispatcher* const game_events) = 0;

	/**
	 * @brief Returns true if this scene prevents underlying scenes from rendering
	 */
	virtual bool opaque() const noexcept = 0;

	/**
	 * @brief Returns true if this scene prevents underlying scenes from updating
	 */
	virtual bool blocking() const noexcept = 0;
};

#endif
