#include "script_xray.hxx"

void j_script_xray::update() {
    auto script_system { game->systems()->get<j_script_system>() };

    ImGui::Begin("Scripts");
    ImGui::PushID(0);
    ImGui::PushStyleColor(ImGuiCol_Button, COLOR_GREEN);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_GREEN_HOVER);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_GREEN_ACTIVE);
    if (ImGui::Button("[F5] Reload")) {
        script_system->load_from_path(j_script_system::default_script_path);
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    const auto& scripts = script_system->scripts();

    const auto selected_script = ([&scripts, this]() -> const j_script* {
        if (script_id_ == j_id_invalid) {
            return nullptr;
        }
        const auto iter { scripts.find(script_id_) };
        if (iter == scripts.end()) {
            return nullptr;
        }
        return iter->second.get();
    })();

    const auto combo_label = ([selected_script, this]() -> const char* {
        if (script_id_ == j_id_invalid) {
            return "Choose script";
        }
        return selected_script ? selected_script->name().c_str() : "<INVALID>";
    })();

    if (ImGui::BeginCombo("Script", combo_label)) {
        for (const auto& [name, script] : scripts) {
            const bool is_selected { script_id_ == script->id() };
            if (ImGui::Selectable(script->name().c_str(), is_selected)) {
                script_id_ = script->id();
            }
        }
        ImGui::EndCombo();
    }
    if (selected_script) {
        if (ImGui::TreeNode("Globals")) {
            for (const auto& global : selected_script->globals()) {
                ImGui::Text("%s", global.c_str());
            }
            ImGui::TreePop();
        }
    }
}