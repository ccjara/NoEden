#ifndef JARALYN_INVENTORY_SCENE_HXX
#define JARALYN_INVENTORY_SCENE_HXX

#include "../components/components.hxx"
#include "../game_event.hxx"
#include "base_scene.hxx"

/**
 * @brief Renders an inventory of any game unit
 */
class j_inventory_scene : public j_base_scene {
protected:
    const j_scene_type type_ { j_scene_type::inventory };

    entt::entity* entity_ { nullptr };
    entt::registry* registry_ { nullptr };
public:
    j_inventory_scene();

    void configure(entt::registry* registry, entt::entity* entity);
    void update(j_input_state& input) override;
    void render(j_display& display) override;
};

#endif
