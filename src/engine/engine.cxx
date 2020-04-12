#include "engine.hxx"

engine::engine(
    system_factory* const sys_factory,
    scene_factory* const sc_factory
) :
    sys_factory_ { sys_factory },
    scene_loader_ { sc_factory, managers_.platform.get() } {
    assert(sys_factory);
    assert(sc_factory);

    managers_.platform->assign_system_factory(sys_factory_);
}

void engine::run() {
    managers_.startup();

    scene_loader_.load_initial_scene();

    managers_.env->clock().start();

    while (managers_.env->is_running()) {
        managers_.env->clock().tick();

        managers_.platform->process_events();

        managers_.task->process(managers_.platform->systems());

        managers_.state->distribute_changes();
    }

    managers_.shutdown();
}
