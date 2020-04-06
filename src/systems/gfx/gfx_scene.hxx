#ifndef JARALYN_GFX_SCENE_HXX
#define JARALYN_GFX_SCENE_HXX

#include "../../engine/framework/system_scene.hxx"
#include "gfx_task.hxx"
#include "renderer.hxx"

class gfx_scene : public system_scene {
public:
    gfx_scene(renderer* r);
};

#endif
