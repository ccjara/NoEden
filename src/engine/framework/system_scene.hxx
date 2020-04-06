#ifndef JARALYN_SYSTEM_SCENE_HXX
#define JARALYN_SYSTEM_SCENE_HXX

#include "object.hxx"
#include "scene.hxx"
#include "task.hxx"

class system_scene : public scene {
protected:
    std::unique_ptr<task> primary_task_;
public:
    void create_object(object&& obj) {
        objects_[obj.type()].emplace_back(&obj);
    }

    const task& primary_task() const noexcept {
        return *primary_task_;
    }
};

#endif
