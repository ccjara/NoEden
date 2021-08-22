#ifndef JARALYN_AI_SYSTEM_HXX
#define JARALYN_AI_SYSTEM_HXX

#include "../game.hxx"
#include "../components/components.hxx"
#include "../components/item.hxx"
#include "../event/world_event.hxx"
#include "../system.hxx"

#include "ai_node.hxx"

/**
 * @brief Acts on units to give them specific behaviors
 */
class j_ai_system : public j_system<j_ai_system> {
private:
    void on_world_step(const j_world_step_event& e);
public:
    void on_load() override;
    void update(uint32_t delta_time) override;
};

#endif
