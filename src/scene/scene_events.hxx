#ifndef JARALYN_SCENE_EVENTS_HXX
#define JARALYN_SCENE_EVENTS_HXX

class Scene;

struct SceneLoadedEvent {
    SceneLoadedEvent(Scene* scene) : scene { scene } {
        assert(scene);
    }

    Scene* scene { nullptr };
};

#endif
