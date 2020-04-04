#include "engine.hxx"

engine::engine(
    system_factory* const sys_factory,
    scene_factory* const sc_factory
) :
    sys_factory_ { sys_factory },
    scene_loader_ { std::make_unique<scene_loader>(sc_factory, platform_.get()) } {
    assert(sys_factory_);
    assert(sc_factory);
}

void engine::startup() noexcept {
    resource_->startup();
    env_->startup(*resource_);
    platform_->startup(sys_factory_, &managers_);
    task_->startup(*platform_);
}

void engine::shutdown() noexcept {
    task_->shutdown();
    platform_->shutdown();
    env_->shutdown();
    resource_->shutdown();
}

void engine::run() {
    scene_loader_->load_initial_scene();

    while (env_->is_running()) {
        platform_->process_events(*env_);

        task_->process(platform_->systems());
    }
}
