#ifndef JARALYN_SCENE_HXX
#define JARALYN_SCENE_HXX



class j_scene {
protected:
	j_scene_id id_ { 0 };
	entt::registry registry_;
public:
	virtual ~j_scene() = default;

	virtual void render() = 0;
};

#endif
