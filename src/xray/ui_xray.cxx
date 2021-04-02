#include "ui_xray.hxx"

/**
 * @brief Recursively renders the given node and its anchored nodes
 *
 * Returns false if anchoring has altered and recursive rendering must
 * be cancelled due to reference invalidation.
 */
bool render_anchor(j_ui_node* node) {
    if (!node) {
        return true;
    }
    const auto& anchored_by { node->anchored_by() };
    const bool has_anchors { !anchored_by.empty() };
    const auto imgui_tree_node_flags {
        has_anchors ? ImGuiTreeNodeFlags_None : ImGuiTreeNodeFlags_Leaf
    };
    const auto id { node->id().data() };

    if (ImGui::TreeNodeEx(id, imgui_tree_node_flags, id)) {
        if (ImGui::BeginDragDropTarget()) {
            if (auto payload = ImGui::AcceptDragDropPayload("_TREENODE")) {
                j_ui_node* accepted_node { nullptr };
                std::memcpy(&accepted_node, payload->Data, sizeof(j_ui_node*));
                if (accepted_node->can_anchor_to(node)) {
                    accepted_node->anchor_to(*node);
                    ImGui::EndDragDropTarget();
                    ImGui::TreePop();
                    return false;
                }
            }
            ImGui::EndDragDropTarget();
        }
        if (!node->is_root() && ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload("_TREENODE", &node, sizeof(j_ui_node*));
            ImGui::Text("Drag to a new anchor node");
            ImGui::EndDragDropSource();
        }
        for (const auto& n : anchored_by) {
            if (!render_anchor(n)) {
                ImGui::TreePop();
                return false;
            }
        }
        ImGui::TreePop();
    }
    return true;
}

void render_hierarchy(j_ui_node* node) {
    if (!node) {
        return;
    }
    if (ImGui::TreeNode(node->id().data())) {
        for (const auto& n : node->children()) {
            render_hierarchy(n);
        }
        ImGui::TreePop();
    }
}

void j_ui_xray::update() {
    ImGui::Begin("UI");

    auto hud_system { game->systems()->get<j_hud_system>() };

    if (ImGui::CollapsingHeader("Hierarchy")) {
        render_hierarchy(hud_system->ui_tree().root());
    }

    if (ImGui::CollapsingHeader("Anchors")) {
        render_anchor(hud_system->ui_tree().root());
    }

    ImGui::End();
}
