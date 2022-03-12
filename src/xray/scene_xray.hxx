#ifndef JARALYN_SCENE_XRAY_HXX
#define JARALYN_SCENE_XRAY_HXX

#include "xray_interface.hxx"
#include "../scene/scene.hxx"
#include "../scene/scene_events.hxx"

class SceneXray : public IXray {
public:
    explicit SceneXray(EventManager& dispatcher);

    void update() override;
private:
    EventManager& events_;
    Scene* scene_ { nullptr };

    bool on_scene_loaded(SceneLoadedEvent& e);
};

#endif
