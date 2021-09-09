#ifndef JARALYN_SCENE_XRAY_HXX
#define JARALYN_SCENE_XRAY_HXX

#include "xray_interface.hxx"
#include "../scene/scene.hxx"
#include "../scene/scene_events.hxx"

// script xray fixen
// enum casing vereinheitlichen

class SceneXray : public IXray {
public:
    explicit SceneXray(entt::dispatcher& dispatcher);

    void update() override;
private:
    entt::dispatcher& dispatcher_;
    Scene* scene_ { nullptr };

    void on_scene_loaded(const SceneLoadedEvent& e);
};

#endif
