#include "script_xray.hxx"

ScriptXray::ScriptXray(entt::dispatcher& dispatcher, Scripts& scripts) :
    dispatcher_ { dispatcher },
    scripts_ { scripts } {
    dispatcher_.sink<ScriptLoadedEvent>().connect<&ScriptXray::on_script_loaded>(this);
    dispatcher_.sink<ScriptResetEvent>().connect<&ScriptXray::on_script_reset>(this);
}

void ScriptXray::on_script_loaded(const ScriptLoadedEvent& e) {
}

void ScriptXray::on_script_reset(const ScriptResetEvent& e) {
    selected_script_id_ = std::nullopt;
}

void ScriptXray::update() {
    ImGui::Begin("Scripts");
    ImGui::PushID(0);
    ImGui::PushStyleColor(ImGuiCol_Button, COLOR_GREEN);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_GREEN_HOVER);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_GREEN_ACTIVE);
    if (ImGui::Button("[F5] Reload")) {
        scripts_.load_from_path(Scripts::default_script_path);
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    Script* current_script { 
        selected_script_id_ ? scripts_.get_by_id(*selected_script_id_) : nullptr
    };
    const auto combo_label {
         current_script ? current_script->name().c_str() : "Choose script"
    };

    if (ImGui::BeginCombo("Script", combo_label)) {
        for (const auto& [name, script] : scripts_.scripts()) {
            const bool is_selected {
                selected_script_id_ && *selected_script_id_ == script->id
            };
            if (ImGui::Selectable(script->name().c_str(), is_selected)) {
                selected_script_id_ = script->id;
            }
        }
        ImGui::EndCombo();
    }

    render_current_script(current_script);
    ImGui::End();
}

void ScriptXray::render_current_script(Script *current) {
    if (!current) {
        return;
    }
    if (ImGui::TreeNode("Globals")) {
        for (const auto& global : current->globals()) {
            ImGui::Text("%s", global.c_str());
        }
        ImGui::TreePop();
    }
}
