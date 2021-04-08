#include "entity_xray.hxx"

void j_entity_xray::update() {
    ImGui::Begin("Entities");

    auto reg { game->entities() };

    reg->view<jc_item, jc_object_descriptor>().each([](
        entt::entity entity,
        const auto& item,
        const auto& desc
    ) {
        ImGui::Text("[%i] %s", entity, desc.label.c_str());
    });

    ImGui::End();
}
