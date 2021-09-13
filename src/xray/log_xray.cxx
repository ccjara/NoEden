#include "log_xray.hxx"

void LogXray::update() {
    static bool auto_scroll { true };

    ImGui::Begin("Log");

    ImGui::BeginChild("scrolling", ImVec2(), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2());

    auto& logs { Log::logs_ };

    for (auto& entry : logs) {
        ImGui::TextUnformatted(entry.message.c_str(), entry.message.c_str() + entry.message.size());
    }

    ImGui::PopStyleVar();

    if (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();

    ImGui::End();
}
