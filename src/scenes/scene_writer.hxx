#ifndef JARALYN_SCENE_WRITER_HXX
#define JARALYN_SCENE_WRITER_HXX

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

#endif
