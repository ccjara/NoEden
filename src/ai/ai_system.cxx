#include "ai_system.hxx"

void j_ai_system::on_load() {
    events_->bind<j_world_step_event, &j_ai_system::on_world_step>(this);
}

void j_ai_system::on_world_step(const j_world_step_event& e) {
    LOG(INFO) << "WORLD ADVANCES " << e.steps << " STEPS";

    auto ai_group { game->entities()->view<jc_ai>() };

    for (auto ai_entity : ai_group) {
        ai_group.get<jc_ai>(ai_entity).root->visit();
    }
}

void j_ai_system::update(uint32_t delta_time) {
    events_->process();
}
