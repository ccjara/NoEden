#ifndef JARALYN_SYSTEM_SCENE_HXX
#define JARALYN_SYSTEM_SCENE_HXX

#include "object.hxx"
#include "scene.hxx"

class system_scene : public scene {
public:
    void create_object(object&& obj) override;
};

#endif
