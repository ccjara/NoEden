#ifndef JARALYN_STATUS_SCENE
#define JARALYN_STATUS_SCENE

#include "../components.hxx"
#include "base_scene.hxx"

class j_status_scene : public j_base_scene {
protected:
    entt::entity* entity_ { nullptr };
    entt::registry* registry_ { nullptr };

    std::pair<std::string_view, j_color> attribute_info(const j_attribute& attr);
public:
    j_status_scene();

    void configure(entt::registry* registry, entt::entity* entity);
    void update(j_input_state& input) override;
    void render(j_display& display) override;
};

#endif
