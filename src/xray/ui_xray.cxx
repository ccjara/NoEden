#include "ui_xray.hxx"

UiXray::UiXray(EventManager& dispatcher, Ui& ui) :
    ui_ { ui },
    events_ { dispatcher } {
}

bool UiXray::render_anchor(UiNode* node) {
    if (!node) {
        return true;
    }
    const auto& anchored_by { node->anchored_by() };
    const bool has_anchors { !anchored_by.empty() };
    const auto imgui_tree_node_flags {
        has_anchors ? ImGuiTreeNodeFlags_None : ImGuiTreeNodeFlags_Leaf
    };
    const int int_id = 0; // is this legal? renders one node per invocation so there is a different stack address each time
    if (ImGui::TreeNodeEx(static_cast<const void*> (&int_id), imgui_tree_node_flags, "%s", node->id().c_str())) {
        if (ImGui::BeginDragDropTarget()) {
            if (auto payload = ImGui::AcceptDragDropPayload("_TREENODE")) {
                UiNode* accepted_node { nullptr };
                std::memcpy(&accepted_node, payload->Data, sizeof(UiNode*));
                if (accepted_node->can_anchor_to(node)) {
                    accepted_node->anchor_to(node);
                    ImGui::EndDragDropTarget();
                    ImGui::TreePop();
                    return false;
                }
            }
            ImGui::EndDragDropTarget();
        }
        if (!node->is_root() && ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload("_TREENODE", &node, sizeof(UiNode*));
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

bool UiXray::render_hierarchy(UiNode* node) {
    if (!node) {
        return false;
    }
    const int int_id = 0; // is this legal? see render_anchor() comment
    if (ImGui::TreeNode(static_cast<const void*>(&int_id), "%s", node->id().c_str())) {
        for (const auto& n : node->children()) {
            render_hierarchy(n);
        }
        ImGui::TreePop();
    }
    return true;
}

void UiXray::update() {
    ImGui::Begin("UI");

    if (ImGui::CollapsingHeader("Hierarchy")) {
        render_hierarchy(ui_.ui_tree().root());
    }

    if (ImGui::CollapsingHeader("Anchors")) {
        render_anchor(ui_.ui_tree().root());
    }

    ImGui::End();
}
