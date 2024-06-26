#include "xray/ui_xray.hxx"
#include "ui/ui.hxx"

bool UiXray::render_anchor(UiNode* node) {
    if (!node) {
        return true;
    }
    const auto& anchored_by { node->anchored_by() };
    const bool has_anchors { !anchored_by.empty() };
    ImGuiTreeNodeFlags imgui_tree_node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if (!has_anchors) {
        imgui_tree_node_flags |= ImGuiTreeNodeFlags_Leaf;
    };
    if (selected_node_id_ == node->id()) {
        imgui_tree_node_flags |= ImGuiTreeNodeFlags_Selected;
    }

    const int int_id = 0; // is this legal? renders one node per invocation so there is a different stack address each time
    const bool is_tree_node_open = ImGui::TreeNodeEx(static_cast<const void*> (&int_id), imgui_tree_node_flags, "%s", node->id().c_str());
    if (ImGui::IsItemClicked()) {
        selected_node_id_ = node->id();
    }
    if (is_tree_node_open) {
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

void UiXray::render() {
    ImGui::Begin("UI");

    if (ImGui::CollapsingHeader("Hierarchy")) {
        render_hierarchy(Ui::tree().root());
    }

    if (ImGui::CollapsingHeader("Anchors")) {
        render_anchor(Ui::tree().root());
    }

    if (!selected_node_id_.empty()) {
        UiNode* n = Ui::tree().get_node_by_id(selected_node_id_);
        if (n) {
            ImGui::Text("Id: %s", n->id().c_str());
            i32 size_buf[2] = {
                static_cast<i32> (n->size().x),
                static_cast<i32> (n->size().y)
            };
            i32 rel_pos_buf[2] = {
                static_cast<i32> (n->relative_position().x),
                static_cast<i32> (n->relative_position().y)
            };
            if (ImGui::InputInt2("Size", size_buf)) {
                n->resize(
                    static_cast<u32> (std::max(0, size_buf[0])),
                    static_cast<u32> (std::max(0, size_buf[1]))
                );
            }
            if (ImGui::InputInt2("Rel. position", rel_pos_buf)) {
                n->move(rel_pos_buf[0], rel_pos_buf[1]);
            }
        } else {
            selected_node_id_ = "";
        }

    }

    ImGui::End();
}
